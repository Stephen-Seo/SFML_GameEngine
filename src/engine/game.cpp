
#include "game.hpp"

#ifdef GAME_NO_RENDER_WINDOW
#include <GL/glew.h>
#endif

#ifndef NDEBUG
  #include <iostream>
#endif

// set packfile name/filepath if one is being used
#define PACKFILE_NAME ""

// set to true if a packfile is being used
#define IS_USING_PACKFILE false

// if not using cmake to build and using the ResourcePacker lib,
// define ResourcePacker_FOUND


#if defined(ResourcePacker_FOUND)
#else
#  define IS_USING_PACKFILE false
#endif

#if IS_USING_PACKFILE == true
#  define RESOURCE_MANAGER_MODE GameResources::PACKFILE
#else
#  define RESOURCE_MANAGER_MODE GameResources::DEFAULT
#endif

Game::Game()
: window(sf::VideoMode(720,480), "SFML App"),
resourceManager(&stateStack, RESOURCE_MANAGER_MODE, PACKFILE_NAME),
mPlayer(),
sPlayer(),
stateStack(),
context(window, resourceManager, mPlayer, sPlayer, ecEngine, isQuitting, connection, clearColor),
isQuitting(false),
connection()
{
    frameTime = sf::seconds(1.f / 60.f);

#ifdef GAME_NO_RENDER_WINDOW
    sf::ContextSettings settings = window.getSettings();

    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.majorVersion = 3;
    settings.minorVersion = 2;

    window.create(sf::VideoMode(720,480), "SFML App", sf::Style::Default, settings);
  #ifndef NDEBUG
    settings = window.getSettings();

    std::cout << "depth bits:" << settings.depthBits << std::endl;
    std::cout << "stencil bits:" << settings.stencilBits << std::endl;
    std::cout << "antialiasing level:" << settings.antialiasingLevel << std::endl;
    std::cout << "version:" << settings.majorVersion << "." << settings.minorVersion << std::endl;
  #endif
#endif
}

void Game::run()
{
    stateStack.pushState(startingState);

    sf::Clock clock;
    sf::Time lastUpdateTime = sf::Time::Zero;
    while (window.isOpen() && !isQuitting)
    {
        lastUpdateTime += clock.restart();
        while (lastUpdateTime > frameTime)
        {
            lastUpdateTime -= frameTime;
            processEvents();
            update(frameTime);
        }
        draw();
    }

    if(window.isOpen())
        window.close();
}

void Game::setStartingState(const std::string& stateName)
{
    startingState = stateName;
}

void Game::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        stateStack.handleEvent(event, context);
        if(event.type == sf::Event::Closed)
            window.close();
    }
}

void Game::update(sf::Time deltaTime)
{
    stateStack.update(deltaTime, context);
    if(connection)
    {
        connection->update(deltaTime);
    }
}

void Game::draw()
{
#ifndef GAME_NO_RENDER_WINDOW
    window.clear(clearColor);
#else
    float r = (float) clearColor.r / 255.0f;
    float g = (float) clearColor.g / 255.0f;
    float b = (float) clearColor.b / 255.0f;
    float a = (float) clearColor.a / 255.0f;
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
#endif
    stateStack.draw(context);
    window.display();
}

