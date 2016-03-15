
#include "game.hpp"

#include <iostream>
#include <cassert>

Game::Game()
: window(sf::VideoMode(960,540), "SFML App", sf::Style::Titlebar | sf::Style::Close),
resourceManager(&stateStack),
mPlayer(),
sPlayer(),
stateStack(),
context(window, resourceManager, mPlayer, sPlayer, isQuitting, connection, clearColor, randomEngine),
isQuitting(false),
connection(),
randomEngine(std::random_device()())
{
    frameTime = sf::seconds(1.f / 60.f);
}

Game::~Game()
{
}

void Game::run()
{
    stateStack.pushState(startingState);

    sf::Time lastUpdateTime = sf::Time::Zero;

    sf::Clock clock;

    while(window.isOpen() && !isQuitting)
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
    window.clear(clearColor);
    stateStack.draw(context);
    window.display();
}

