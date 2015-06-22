
#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <memory>
#include <SFML/Graphics.hpp>

#include "resourceIdentifiers.hpp"

class ResourceManager;
class MusicPlayer;
class SoundPlayer;
class Engine;
class Connection;

struct Context
{
    Context(sf::RenderWindow& window,
            ResourceManager& resourceManager,
            MusicPlayer& mPlayer,
            SoundPlayer& sPlayer,
            Engine& ecEngine,
            bool& isQuitting,
            std::unique_ptr<Connection>& connection);

    sf::RenderWindow* window;
    ResourceManager* resourceManager;
    MusicPlayer* mPlayer;
    SoundPlayer* sPlayer;
    Engine* ecEngine;
    bool* isQuitting;
    std::unique_ptr<Connection>* connection;
};

#endif
