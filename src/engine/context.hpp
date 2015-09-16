
#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <memory>
#include <random>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "resourceIdentifiers.hpp"

#ifdef GAME_USE_GLFW
class GLFWwindow;
#endif
class ResourceManager;
class MusicPlayer;
class SoundPlayer;
class Engine;
class Connection;

struct Context
{
#ifdef GAME_USE_GLFW
    Context(GLFWwindow* window,
#else
    Context(sf::Window* window,
#endif
            ResourceManager& resourceManager,
            MusicPlayer& mPlayer,
            SoundPlayer& sPlayer,
            Engine& ecEngine,
            bool& isQuitting,
            std::unique_ptr<Connection>& connection,
            sf::Color& clearColor,
            std::mt19937& randomEngine);

#ifdef GAME_USE_GLFW
    GLFWwindow* window;
#else
    sf::Window* window;
#endif
    ResourceManager* resourceManager;
    MusicPlayer* mPlayer;
    SoundPlayer* sPlayer;
    Engine* ecEngine;
    bool* isQuitting;
    std::unique_ptr<Connection>* connection;
    sf::Color* clearColor;
    std::mt19937* randomEngine;
};

#endif
