
#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "resourceHolder.hpp"
#include "resourceIdentifiers.hpp"
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
    void registerStates();

    sf::RenderWindow window;
    sf::Time frameTime;

    TextureHolder textureHolder;
    FontHolder fontHolder;

    MusicPlayer mPlayer;
    SoundPlayer sPlayer;

    StateStack stateStack;
};

#endif
