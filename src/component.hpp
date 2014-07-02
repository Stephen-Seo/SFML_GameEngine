
#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <memory>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>

class ComponentHolder;

class Component
{
public:
    typedef std::unique_ptr<Component> Ptr;

    Component(ComponentHolder* holder);
    virtual ~Component() {}

    virtual void update(sf::Time dt) {}
    virtual void handleEvent(const sf::Event& event) {}
    virtual void draw() {}

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
    ComponentHolder* holder;

};

#endif
