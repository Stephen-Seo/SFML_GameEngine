
#include "state.hpp"
#include "stateStack.hpp"

State::Context::Context(sf::RenderWindow& window, ResourceManager& resourceManager, MusicPlayer& mPlayer, SoundPlayer& sPlayer, bool& isQuitting) :
window(&window),
resourceManager(&resourceManager),
mPlayer(&mPlayer),
sPlayer(&sPlayer),
isQuitting(&isQuitting)
{}

State::State(StateStack& stack, Context context)
: stack(&stack), context(context)
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

void State::requestStateClear()
{
    stack->clearStates();
}

State::Context State::getContext() const
{
    return context;
}

