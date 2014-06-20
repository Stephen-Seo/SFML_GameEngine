
#include "state.hpp"
#include "stateStack.hpp"

State::Context::Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, MusicPlayer& mPlayer, SoundPlayer& sPlayer, bool& isQuitting) :
window(&window),
textures(&textures),
fonts(&fonts),
mPlayer(&mPlayer),
sPlayer(&sPlayer),
isQuitting(&isQuitting)
{}

State::State(StateStack& stack, Context context)
: stack(&stack), context(context)
{}

State::~State()
{}

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

