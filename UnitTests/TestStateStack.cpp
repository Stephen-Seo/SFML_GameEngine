
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
        tset.insert(static_cast<Textures::ID>(0));
        tset.insert(static_cast<Textures::ID>(1));
        tset.insert(static_cast<Textures::ID>(2));
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

    sf::RenderWindow window;
    MusicPlayer mPlayer;
    SoundPlayer sPlayer;
    Engine ecEngine;
    bool derp;
    std::unique_ptr<Connection> connection;
    sf::Color clearColor;

    Context context(window, rManager, mPlayer, sPlayer, ecEngine, derp, connection, clearColor);


    stack.registerState<TestState>(static_cast<States::ID>(5), context);
    rManager.registerTexture(static_cast<Textures::ID>(0), "UnitTestRes/Test.png");
    rManager.registerTexture(static_cast<Textures::ID>(1), "UnitTestRes/Test.png");
    rManager.registerTexture(static_cast<Textures::ID>(2), "UnitTestRes/Test.png");


    bool success = true;

    try
    {
    // Load resource
    stack.pushState(static_cast<States::ID>(5));
    stack.update(sf::Time::Zero, context);

    // Remove resource
    stack.popState();
    stack.update(sf::Time::Zero, context);

    // Load resource again
    stack.pushState(static_cast<States::ID>(5));
    stack.update(sf::Time::Zero, context);

    // Remove resource again
    stack.popState();
    stack.update(sf::Time::Zero, context);
    } catch (const std::exception& e)
    {
        std::cout << "EXCEPTION WHILE HANDLING RESOURCES:\n\t" << e.what() << "\n" << std::flush;
        success = false;
    }

    ASSERT_TRUE(success);

}
