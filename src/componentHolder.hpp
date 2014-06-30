
#ifndef COMPONENT_HOLDER_HPP
#define COMPONENT_HOLDER_HPP

#define MESSAGES_MAX_SIZE 32

#include <cassert>
#include <list>

#include "component.hpp"

class ComponentHolder
{
public:
    ComponentHolder();
    virtual ~ComponentHolder();

    void distributeMessages();
    void updateComponents(sf::Time dt);
    void sendEventToComponents(const sf::Event& event);
    void drawComponents();

    void queueMessage(int message);
protected:
    void addComponent(Component* component);

private:
    std::list<Component::Ptr> components;

    int mqueue[MESSAGES_MAX_SIZE];
    int mhead;
    int mtail;
};

#endif
