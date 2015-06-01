
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

    StateStack();

    template <class T>
    void registerState(States::ID stateID, Context context);

    void update(sf::Time dt, Context context);
    void draw(Context context);
    void handleEvent(const sf::Event& event, Context context);

    void pushState(States::ID stateID);
    void popState();
    void clearStates();

    bool isEmpty() const;

    ResourcesSet getNeededResources();
private:
    State::Ptr createState(States::ID stateID);
    void applyPendingChanges(Context context);

    struct PendingChange
    {
        explicit PendingChange(Action action, States::ID stateID = States::None);
        Action action;
        States::ID stateID;
    };

    std::vector<State::Ptr> stack;
    std::vector<PendingChange> pendingList;
    std::map<States::ID, std::function<State::Ptr()>> factories;
};

template <class T>
void StateStack::registerState(States::ID stateID, Context context)
{
    factories[stateID] = [this, context] ()
    {
        return State::Ptr(new T(*this, context));
    };
}

#endif
