
#ifndef STATE_HPP
#define STATE_HPP

#include <SFML/Graphics.hpp>

#include "resourceHolder.hpp"
#include "resourceIdentifiers.hpp"

class StateStack;

class State
{
public:
    typedef std::unique_ptr<State> Ptr;

    struct Context
    {
        Context(sf::RenderWindow& window,
                TextureHolder& textures,
                FontHolder& fonts);

        sf::RenderWindow* window;
        TextureHolder* textures;
        FontHolder* fonts;
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
