
#ifndef STATE_HPP
#define STATE_HPP

#include <memory>

#include <SFML/Graphics.hpp>

#include "context.hpp"
#include "resourceIdentifiers.hpp"
#include "stateIdentifiers.hpp"

class ResourceManager;

class StateStack;

class State
{
public:
    typedef std::unique_ptr<State> Ptr;

    State(StateStack& stack, Context context);
    virtual ~State();

    virtual void draw() = 0;

    // Return true to continue update down the StateStack
    virtual bool update(sf::Time dt) = 0;

    // Return true to continue handleEvent down the StateStack
    virtual bool handleEvent(const sf::Event& event) = 0;

    ResourcesSet getNeededResources();
protected:
    friend class GuiSystem;

    void requestStackPush(States::ID stateID);
    void requestStackPop();
    void requestStackClear();

    Context getContext() const;

    TextureSet tset;
    FontSet fset;
    SoundSet sset;
private:
    StateStack* stack;
    Context context;
};

#endif
