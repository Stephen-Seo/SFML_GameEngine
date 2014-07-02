
#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

template <class CT>
class ComponentHolder;

template <class C>
class Component
{
public:
    typedef std::unique_ptr<Component<C>> Ptr;

    Component(ComponentHolder<C>* holder);
    virtual ~Component() {}

    virtual void update(sf::Time dt, C context) {}
    virtual void handleEvent(const sf::Event& event) {}
    virtual void draw(sf::RenderWindow window) {}

    virtual void receiveMessage(int message) {}

    bool requiresUpdate();
    bool requiresEvents();
    bool requiresDraw();
    bool requiresMessage();

protected:
    void broadcastMessage(int message);

    bool reqUpdate;
    bool reqEvents;
    bool reqDraw;
    bool reqMessage;

private:
    ComponentHolder<C>* holder;

};

#include "component.inl"

#endif
