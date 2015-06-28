
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
private:
    void processEvents();
    void update(sf::Time deltaTime);
    void draw();

    void registerResources();
    void registerStates();

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
};

#endif
