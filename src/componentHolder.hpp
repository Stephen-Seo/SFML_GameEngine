
#ifndef COMPONENT_HOLDER_HPP
#define COMPONENT_HOLDER_HPP

#define MESSAGES_MAX_SIZE 32

#include <cassert>
#include <list>

#include "component.hpp"

template <class CT>
class ComponentHolder
{
public:
    ComponentHolder();
    virtual ~ComponentHolder();

    void distributeMessages();
    void updateComponents(sf::Time dt, CT context);
    void sendEventToComponents(const sf::Event& event);
    void drawComponents(sf::RenderWindow window);

    void queueMessage(int message);
protected:
    void addComponent(Component<CT>* component);

private:
    std::list<typename Component<CT>::Ptr> components;

    int mqueue[MESSAGES_MAX_SIZE];
    int mhead;
    int mtail;
};

#include "componentHolder.inl"

#endif
