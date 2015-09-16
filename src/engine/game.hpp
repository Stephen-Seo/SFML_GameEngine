
#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include <random>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#ifdef GAME_USE_GLFW
  #include <GL/glew.h>
  #include <GLFW/glfw3.h>
  #include <vector>
#endif

#include "ec/engine.hpp"
#include "context.hpp"
#include "connection.hpp"
#include "resourceHolder.hpp"
#include "resourceIdentifiers.hpp"
#include "resourceManager.hpp"
#include "musicPlayer.hpp"
#include "soundPlayer.hpp"
#include "state.hpp"
#include "stateStack.hpp"

class Game
{
public:
    Game();
    ~Game();

    void run();

    template<class SubState>
    void registerState(const std::string& stateName);

    void setStartingState(const std::string& stateName);

private:
    void processEvents();
    void update(sf::Time deltaTime);
    void draw();

#ifdef GAME_USE_GLFW
    GLFWwindow* window;

    std::vector<sf::Event> events;

public:
    static std::vector<sf::Event>* staticEvents;

private:
#else
  #ifndef GAME_NO_RENDER_WINDOW
    sf::RenderWindow window;
  #else
    sf::Window window;
  #endif
#endif

    sf::Time frameTime;

    ResourceManager resourceManager;

    MusicPlayer mPlayer;
    SoundPlayer sPlayer;

    StateStack stateStack;

    Context context;

    bool isQuitting;

    Engine ecEngine;

    std::unique_ptr<Connection> connection;

    sf::Color clearColor;

    std::string startingState;

    std::mt19937 randomEngine;
};

template<class SubState>
void Game::registerState(const std::string& stateName)
{
    stateStack.registerState<SubState>(stateName, context);
}

#ifdef GAME_USE_GLFW
namespace GameCallbacks
{
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void charCallback(GLFWwindow* window, unsigned int codepoint);
    void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    void cursorEnterCallback(GLFWwindow* window, int entered);
    void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
}
#endif

#endif
