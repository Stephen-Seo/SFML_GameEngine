
#include "state.hpp"
#include "stateStack.hpp"

State::State(StateStack& stack, Context context)
: stack(&stack)
{}

State::~State()
{}

void State::requestStackPush(const std::string& stateName)
{
    stack->pushState(stateName);
}

void State::requestStackPop()
{
    stack->popState();
}

void State::requestStackClear()
{
    stack->clearStates();
}

