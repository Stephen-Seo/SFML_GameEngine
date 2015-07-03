
#include "gtest/gtest.h"

#include <exception>
#include <iostream>
#include <memory>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <engine/state.hpp>
#include <engine/stateStack.hpp>
#include <engine/resourceManager.hpp>
#include <engine/musicPlayer.hpp>
#include <engine/soundPlayer.hpp>
#include <engine/ec/engine.hpp>
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
    ResourceManager rManager(&stack, GameResources::DEFAULT);

    MusicPlayer mPlayer;
    SoundPlayer sPlayer;
    Engine ecEngine;
    bool derp;
    std::unique_ptr<Connection> connection;
    sf::Color clearColor;

    Context context(nullptr, rManager, mPlayer, sPlayer, ecEngine, derp, connection, clearColor);


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
