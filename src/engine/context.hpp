
#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <memory>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "resourceIdentifiers.hpp"

class ResourceManager;
class MusicPlayer;
class SoundPlayer;
class Engine;
class Connection;

struct Context
{
    Context(sf::Window& window,
            ResourceManager& resourceManager,
            MusicPlayer& mPlayer,
            SoundPlayer& sPlayer,
            Engine& ecEngine,
            bool& isQuitting,
            std::unique_ptr<Connection>& connection,
            sf::Color& clearColor);

    sf::Window* window;
    sf::RenderWindow* renderWindow;
    ResourceManager* resourceManager;
    MusicPlayer* mPlayer;
    SoundPlayer* sPlayer;
    Engine* ecEngine;
    bool* isQuitting;
    std::unique_ptr<Connection>* connection;
    sf::Color* clearColor;
};

#endif
