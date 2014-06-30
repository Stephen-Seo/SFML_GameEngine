
#ifndef STATE_HPP
#define STATE_HPP

#include <SFML/Graphics.hpp>

#include "resourceHolder.hpp"
#include "resourceIdentifiers.hpp"
#include "stateIdentifiers.hpp"
#include "musicPlayer.hpp"
#include "soundPlayer.hpp"

class ResourceManager;

class StateStack;

class State
{
public:
    typedef std::unique_ptr<State> Ptr;

    struct Context
    {
        Context(sf::RenderWindow& window,
                ResourceManager& resourceManager,
                MusicPlayer& mPlayer,
                SoundPlayer& sPlayer,
                bool& isQuitting);

        sf::RenderWindow* window;
        ResourceManager* resourceManager;
        MusicPlayer* mPlayer;
        SoundPlayer* sPlayer;
        bool* isQuitting;
    };

    State(StateStack& stack, Context context);
    virtual ~State();

    virtual void draw() = 0;
    virtual bool update(sf::Time dt) = 0;
    virtual bool handleEvent(const sf::Event& event) = 0;

    ResourcesSet getNeededResources();
protected:
    friend class GuiSystem;

    void requestStackPush(States::ID stateID);
    void requestStackPop();
    void requestStateClear();

    Context getContext() const;

    TextureSet tset;
    FontSet fset;
    SoundSet sset;
private:
    StateStack* stack;
    Context context;
};

#endif
