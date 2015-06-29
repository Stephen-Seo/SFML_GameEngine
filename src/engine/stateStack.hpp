
#ifndef STATE_STACK_HPP
#define STATE_STACK_HPP

#include <cassert>
#include <functional>
#include <vector>
#include <unordered_map>
#include <cstdlib>

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

    std::vector<State::Ptr> stack;
    std::vector<PendingChange> pendingList;
    std::unordered_map<std::string, std::function<State::Ptr()>> factories;
};

template <class T>
void StateStack::registerState(const std::string& stateName, Context context)
{
    factories[stateName] = [this, context] ()
    {
        return State::Ptr(new T(*this, context));
    };
}

#endif
