
#include "componentHolder.hpp"

template <class C>
Component<C>::Component(ComponentHolder<C>* holder) :
reqUpdate(false),
reqEvents(false),
reqDraw(false),
reqMessage(false),
holder(holder)
{}

template <class C>
bool Component<C>::requiresUpdate()
{
    return reqUpdate;
}

template <class C>
bool Component<C>::requiresEvents()
{
    return reqEvents;
}

template <class C>
bool Component<C>::requiresDraw()
{
    return reqDraw;
}

template <class C>
bool Component<C>::requiresMessage()
{
    return reqMessage;
}

template <class C>
void Component<C>::broadcastMessage(int message)
{
    holder->queueMessage(message);
}
