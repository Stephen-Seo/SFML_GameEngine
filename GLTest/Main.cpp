
#include <engine/game.hpp>

#include "TestState.hpp"

int main()
{
    Game game;
    game.registerState<TestState>("test");
    game.setStartingState("test");
    game.run();
}

