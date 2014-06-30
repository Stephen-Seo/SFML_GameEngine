
#include "componentHolder.hpp"

ComponentHolder::ComponentHolder() :
components(),
mqueue(),
mhead(0),
mtail(0)
{}

ComponentHolder::~ComponentHolder()
{}

void ComponentHolder::distributeMessages()
{
    if(mhead != mtail)
    {
        for(auto iter = components.begin(); iter != components.end(); ++iter)
        {
            if((*iter)->requiresMessage())
            {
                for(int tmphead = mhead; tmphead != mtail; tmphead = (tmphead + 1) % MESSAGES_MAX_SIZE)
                {
                    (*iter)->receiveMessage(mqueue[tmphead]);
                }
            }
        }
        mhead = mtail;
    }
}

void ComponentHolder::updateComponents(sf::Time dt)
{
    for(auto iter = components.begin(); iter != components.end(); ++iter)
    {
        if((*iter)->requiresUpdate())
            (*iter)->update(dt);
    }
}

void ComponentHolder::sendEventToComponents(const sf::Event& event)
{
    for(auto iter = components.begin(); iter != components.end(); ++iter)
    {
        if((*iter)->requiresEvents())
            (*iter)->handleEvent(event);
    }
}

void ComponentHolder::drawComponents()
{
    for(auto iter = components.begin(); iter != components.end(); ++iter)
    {
        if((*iter)->requiresDraw())
            (*iter)->draw();
    }
}

void ComponentHolder::queueMessage(int message)
{
    assert((mtail + 1) % MESSAGES_MAX_SIZE != mhead);

    mqueue[mtail] = message;
    mtail = (mtail + 1) % MESSAGES_MAX_SIZE;
}

void ComponentHolder::addComponent(Component* component)
{
    components.push_back(Component::Ptr(component));
}
