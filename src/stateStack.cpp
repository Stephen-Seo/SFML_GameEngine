
#include "stateStack.hpp"

StateStack::PendingChange::PendingChange(Action action, States::ID stateID)
: action(action), stateID(stateID)
{
}

StateStack::StateStack(State::Context context)
: stack(), pendingList(), context(context), factories()
{
}

void StateStack::update(sf::Time dt)
{
    for(auto iter = stack.rbegin(); iter != stack.rend(); ++iter)
    {
        if(!(*iter)->update(dt))
            break;
    }
    applyPendingChanges();
}

void StateStack::draw()
{
    std::for_each(stack.rbegin(), stack.rend(), [] (State::Ptr& state)
    {
        state->draw();
    } );
}

void StateStack::handleEvent(const sf::Event& event)
{
    for(auto iter = stack.rbegin(); iter != stack.rend(); ++iter)
    {
        if(!(*iter)->handleEvent(event))
            break;
    }
    applyPendingChanges();
}

void StateStack::pushState(States::ID stateID)
{
    pendingList.push_back(PendingChange(StateStack::Push, stateID));
}

void StateStack::popState()
{
    pendingList.push_back(PendingChange(StateStack::Pop));
}

void StateStack::clearStates()
{
    pendingList.push_back(PendingChange(StateStack::Clear));
}

bool StateStack::isEmpty() const
{
    return stack.empty();
}

State::Ptr StateStack::createState(States::ID stateID)
{
    auto found = factories.find(stateID);
    assert(found != factories.end());
    return found->second();
}

void StateStack::applyPendingChanges()
{
    std::for_each(pendingList.begin(), pendingList.end(), [this] (PendingChange& change)
    {
        switch (change.action)
        {
        case Push:
            stack.push_back(createState(change.stateID));
            break;
        case Pop:
            stack.pop_back();
            break;
        case Clear:
            stack.clear();
            break;
        }
    } );
    pendingList.clear();
}
