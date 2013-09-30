
#include "game.hpp"

Game::Game()
: window(sf::VideoMode(720,480), "SFML App")
{
    frameTime = sf::seconds(1.f / 60.f);
}

void Game::run()
{
    sf::Clock clock;
    sf::Time lastUpdateTime = sf::Time::Zero;
    while (window.isOpen())
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

void Game::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::KeyPressed:
            handlePlayerInput(event.key.code, true);
            break;
        case sf::Event::KeyReleased:
            handlePlayerInput(event.key.code, false);
            break;
        default:
            break;
        }
    }
}

void Game::update(sf::Time deltaTime)
{
}

void Game::draw()
{
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
}
