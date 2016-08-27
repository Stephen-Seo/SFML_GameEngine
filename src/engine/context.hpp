
#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <memory>
#include <random>
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
    Context(sf::RenderWindow& window,
            ResourceManager& resourceManager,
            MusicPlayer& mPlayer,
            SoundPlayer& sPlayer,
            bool& isQuitting,
            std::unique_ptr<Connection>& connection,
            sf::Color& clearColor,
            std::mt19937& randomEngine,
            void* extraContext);

    sf::RenderWindow* window;
    ResourceManager* resourceManager;
    MusicPlayer* mPlayer;
    SoundPlayer* sPlayer;
    bool* isQuitting;
    std::unique_ptr<Connection>* connection;
    sf::Color* clearColor;
    std::mt19937* randomEngine;
    void* extraContext;
};

#endif
