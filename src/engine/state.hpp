
#ifndef STATE_HPP
#define STATE_HPP

#include <memory>

#include <SFML/Graphics.hpp>

#include "context.hpp"
#include "resourceIdentifiers.hpp"

class ResourceManager;

class StateStack;

class State
{
public:
    typedef std::unique_ptr<State> Ptr;

    State(StateStack& stack, Context context);
    virtual ~State();

    virtual void draw(Context context) = 0;

    // Return true to continue update down the StateStack
    virtual bool update(sf::Time dt, Context context) = 0;

    // Return true to continue handleEvent down the StateStack
    virtual bool handleEvent(const sf::Event& event, Context context) = 0;

protected:
    void requestStackPush(const std::string& stateName);
    void requestStackPop();
    void requestStackClear();

private:
    StateStack* stack;
};

#endif
