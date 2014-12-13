
#ifndef STATE_STACK_HPP
#define STATE_STACK_HPP

#include <cassert>
#include <functional>
#include <vector>

#include <SFML/System.hpp>

#include "state.hpp"
#include "stateIdentifiers.hpp"
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

    explicit StateStack(Context context);

    template <class T>
    void registerState(States::ID stateID);

    void update(sf::Time dt);
    void draw();
    void handleEvent(const sf::Event& event);

    void pushState(States::ID stateID);
    void popState();
    void clearStates();

    bool isEmpty() const;

    ResourcesSet getNeededResources();
private:
    State::Ptr createState(States::ID stateID);
    void applyPendingChanges();

    struct PendingChange
    {
        explicit PendingChange(Action action, States::ID stateID = States::None);
        Action action;
        States::ID stateID;
    };

    std::vector<State::Ptr> stack;
    std::vector<PendingChange> pendingList;
    Context context;
    std::map<States::ID, std::function<State::Ptr()>> factories;
};

template <class T>
void StateStack::registerState(States::ID stateID)
{
    factories[stateID] = [this] ()
    {
        return State::Ptr(new T(*this, context));
    };
}

#endif
