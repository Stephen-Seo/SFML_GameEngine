
#include "stateStack.hpp"

StateStack::StateStack(State::Context context)
{
}

template <class T>
StateStack::registerState(States::ID stateID)
{
    mFactories[stateID] = [this] ()
    {
        return State::Ptr(new T(*this, context));
    };
}

void StateStack::update(sf::Time dt)
{
}

void StateStack::draw()
{
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
}

void StateStack::popState()
{
}

void StateStack::clearStates()
{
}

bool StateStack::isEmpty() const
{
}

State::Ptr StateStack::createState(States::ID stateID)
{
    auto found = factories.find(stateID);
    assert(found != factories.end());
    return found->second();
}

void StateStack::applyPendingChanges()
{
}
