
#include "stateStack.hpp"

StateStack::StateStack(State::Context context)
{
}

template <class T>
StateStack::registerState(States::ID stateID)
{
}

void StateStack::update(sf::Time dt)
{
}

void StateStack::draw()
{
}

void StateStack::handleEvent(const sf::Event& event)
{
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
}

void StateStack::applyPendingChanges()
{
}
