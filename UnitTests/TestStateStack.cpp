
#include "gtest/gtest.h"

#include <exception>
#include <iostream>
#include <memory>
#include <random>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <engine/state.hpp>
#include <engine/stateStack.hpp>
#include <engine/resourceManager.hpp>
#include <engine/musicPlayer.hpp>
#include <engine/soundPlayer.hpp>
#include <engine/context.hpp>
#include <engine/connection.hpp>

class TestState : public State
{
public:
    TestState(StateStack& stack, Context context) : State(stack, context)
    {
        context.resourceManager->registerTexture(*this, "UnitTestRes/Test.png");
        context.resourceManager->registerTexture(*this, "UnitTestRes/Test.png");
    }

    void draw(Context context) {}
    bool update(sf::Time dt, Context context) {return false;}
    bool handleEvent(const sf::Event& event, Context context) {return false;}
};

TEST(StateStackTest, ResourceLoading)
{
    // Initialization
    StateStack stack;
    ResourceManager rManager(&stack);

    sf::RenderWindow window;
    MusicPlayer mPlayer;
    SoundPlayer sPlayer;
    bool derp;
    std::unique_ptr<Connection> connection;
    sf::Color clearColor;
    std::mt19937 randomEngine;

    Context context(window, rManager, mPlayer, sPlayer, derp, connection, clearColor, randomEngine, nullptr);


    stack.registerState<TestState>("test", context);

    bool success = true;

    // Load resource
    stack.pushState("test");
    stack.update(sf::Time::Zero, context);

    // Try unload check
    rManager.unloadCheckResources();

    // Remove resource
    stack.popState();
    stack.update(sf::Time::Zero, context);

    // Load resource again
    stack.pushState("test");
    stack.update(sf::Time::Zero, context);

    // Remove resource again
    stack.popState();
    stack.update(sf::Time::Zero, context);

}
