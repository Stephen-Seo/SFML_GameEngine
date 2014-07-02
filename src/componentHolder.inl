
template <class CT>
ComponentHolder<CT>::ComponentHolder() :
components(),
mqueue(),
mhead(0),
mtail(0)
{}

template <class CT>
ComponentHolder<CT>::~ComponentHolder()
{}

template <class CT>
void ComponentHolder<CT>::distributeMessages()
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

template <class CT>
void ComponentHolder<CT>::updateComponents(sf::Time dt, CT context)
{
    for(auto iter = components.begin(); iter != components.end(); ++iter)
    {
        if((*iter)->requiresUpdate())
            (*iter)->update(dt, context);
    }
}

template <class CT>
void ComponentHolder<CT>::sendEventToComponents(const sf::Event& event)
{
    for(auto iter = components.begin(); iter != components.end(); ++iter)
    {
        if((*iter)->requiresEvents())
            (*iter)->handleEvent(event);
    }
}

template <class CT>
void ComponentHolder<CT>::drawComponents(sf::RenderWindow window)
{
    for(auto iter = components.begin(); iter != components.end(); ++iter)
    {
        if((*iter)->requiresDraw())
            (*iter)->draw(window);
    }
}

template <class CT>
void ComponentHolder<CT>::queueMessage(int message)
{
    assert((mtail + 1) % MESSAGES_MAX_SIZE != mhead);

    mqueue[mtail] = message;
    mtail = (mtail + 1) % MESSAGES_MAX_SIZE;
}

template <class CT>
void ComponentHolder<CT>::addComponent(Component<CT>* component)
{
    components.push_back(typename Component<CT>::Ptr(component));
}
