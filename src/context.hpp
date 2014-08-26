
#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <SFML/Graphics.hpp>

#include "resourceIdentifiers.hpp"

class ResourceManager;
class MusicPlayer;
class SoundPlayer;
class Engine;


struct Context
{
    Context(sf::RenderWindow& window,
            ResourceManager& resourceManager,
            MusicPlayer& mPlayer,
            SoundPlayer& sPlayer,
            Engine& ecEngine,
            bool& isQuitting);

    sf::RenderWindow* window;
    ResourceManager* resourceManager;
    MusicPlayer* mPlayer;
    SoundPlayer* sPlayer;
    Engine* ecEngine;
    bool* isQuitting;
};

#endif
