
#include "state.hpp"
#include "stateStack.hpp"

State::State(StateStack& stack, Context context)
: stack(&stack)
{}

State::~State()
{}

ResourcesSet State::getNeededResources()
{
    return ResourcesSet(tset, fset, sset);
}

void State::requestStackPush(States::ID stateID)
{
    stack->pushState(stateID);
}

void State::requestStackPop()
{
    stack->popState();
}

void State::requestStackClear()
{
    stack->clearStates();
}

