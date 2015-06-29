
#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

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
    void run();

    template<class SubState>
    void registerState(const std::string& stateName);

    void setStartingState(const std::string& stateName);

private:
    void processEvents();
    void update(sf::Time deltaTime);
    void draw();

#ifndef GAME_NO_RENDER_WINDOW
    sf::RenderWindow window;
#else
    sf::Window window;
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
};

template<class SubState>
void Game::registerState(const std::string& stateName)
{
    stateStack.registerState<SubState>(stateName, context);
}

#endif
