
#ifndef STATE_HPP
#define STATE_HPP

#include <SFML/Graphics.hpp>

#include "resourceHolder.hpp"
#include "resourceIdentifiers.hpp"
#include "stateIdentifiers.hpp"
#include "musicPlayer.hpp"
#include "soundPlayer.hpp"

class StateStack;

class State
{
public:
    typedef std::unique_ptr<State> Ptr;

    struct Context
    {
        Context(sf::RenderWindow& window,
                TextureHolder& textures,
                FontHolder& fonts,
                MusicPlayer& mPlayer,
                SoundPlayer& sPlayer);

        sf::RenderWindow* window;
        TextureHolder* textures;
        FontHolder* fonts;
        MusicPlayer* mPlayer;
        SoundPlayer* sPlayer;
    };

    State(StateStack& stack, Context context);
    virtual ~State();

    virtual void draw() = 0;
    virtual bool update(sf::Time dt) = 0;
    virtual bool handleEvent(const sf::Event& event) = 0;

protected:
    void requestStackPush(States::ID stateID);
    void requestStackPop();
    void requestStateClear();

    Context getContext() const;
private:
    StateStack* stack;
    Context context;
};

#endif
