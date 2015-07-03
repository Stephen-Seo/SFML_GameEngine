
#ifndef STATE_STACK_HPP
#define STATE_STACK_HPP

#include <cassert>
#include <functional>
#include <vector>
#include <unordered_map>
#include <memory>
#include <cstdlib>
#include <typeinfo>
#ifndef NDEBUG
  #include <iostream>
#endif

#include <SFML/System.hpp>

#include "state.hpp"
#include "resourceIdentifiers.hpp"

class StateStack : private sf::NonCopyable
{
public:
    enum Action
    {
        Push,
        Pop,
        Clear
    };

    StateStack();

    template <class T>
    void registerState(const std::string& stateName, Context context);

    void update(sf::Time dt, Context context);
    void draw(Context context);
    void handleEvent(const sf::Event& event, Context context);

    void pushState(const std::string& stateID);
    void popState();
    void clearStates();

    bool isEmpty() const;

    std::vector<std::size_t> getContentsHashCodes();

private:
    State::Ptr createState(const std::string& stateID);
    void applyPendingChanges(Context context);

    struct PendingChange
    {
        explicit PendingChange(Action action, std::string stateID = std::string());
        Action action;
        std::string stateID;
    };

    struct FactoryContainer
    {
        std::function<State::Ptr()> factory;
        std::size_t hashCode;
    };

    std::vector<State::Ptr> stack;
    std::vector<PendingChange> pendingList;
    std::unordered_map<std::string, FactoryContainer> factories;
    std::unique_ptr<std::size_t> creatingHashCode;
};

template <class T>
void StateStack::registerState(const std::string& stateName, Context context)
{
    FactoryContainer container;
    container.factory = [this, context] ()
    {
        return State::Ptr(new T(*this, context));
    };
    container.hashCode = typeid(T).hash_code();
#ifndef NDEBUG
    std::cout << "Creating " << typeid(T).name() << ' ' << container.hashCode << std::endl;
#endif

    factories[stateName] = container;
}

#endif
