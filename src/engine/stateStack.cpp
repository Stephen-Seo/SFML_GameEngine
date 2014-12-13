
#include "stateStack.hpp"

#include "resourceManager.hpp"

StateStack::PendingChange::PendingChange(Action action, States::ID stateID)
: action(action), stateID(stateID)
{
}

StateStack::StateStack()
: stack(), pendingList(), factories()
{
}

void StateStack::update(sf::Time dt, Context context)
{
    for(auto iter = stack.rbegin(); iter != stack.rend(); ++iter)
    {
        if(!(*iter)->update(dt, context))
            break;
    }
    applyPendingChanges(context);
}

void StateStack::draw(Context context)
{
    std::for_each(stack.begin(), stack.end(), [context] (State::Ptr& state)
    {
        state->draw(context);
    } );
}

void StateStack::handleEvent(const sf::Event& event, Context context)
{
    for(auto iter = stack.rbegin(); iter != stack.rend(); ++iter)
    {
        if(!(*iter)->handleEvent(event, context))
            break;
    }
    applyPendingChanges(context);
}

void StateStack::pushState(States::ID stateID)
{
    pendingList.push_back(PendingChange(StateStack::Push, stateID));
}

void StateStack::popState()
{
    pendingList.push_back(PendingChange(StateStack::Pop));
}

void StateStack::clearStates()
{
    pendingList.push_back(PendingChange(StateStack::Clear));
}

bool StateStack::isEmpty() const
{
    return stack.empty();
}

ResourcesSet StateStack::getNeededResources()
{
    ResourcesSet resourcesSet;

    for(auto iter = stack.begin(); iter != stack.end(); ++iter)
    {
        ResourcesSet stateSet = (*iter)->getNeededResources();

        for(auto tIter = stateSet.tset.begin(); tIter != stateSet.tset.end(); ++tIter)
        {
            resourcesSet.tset.insert(*tIter);
        }

        for(auto fIter = stateSet.fset.begin(); fIter != stateSet.fset.end(); ++fIter)
        {
            resourcesSet.fset.insert(*fIter);
        }

        for(auto sIter = stateSet.sset.begin(); sIter != stateSet.sset.end(); ++sIter)
        {
            resourcesSet.sset.insert(*sIter);
        }
    }

    return resourcesSet;
}

State::Ptr StateStack::createState(States::ID stateID)
{
    auto found = factories.find(stateID);
    assert(found != factories.end());
    return found->second();
}

void StateStack::applyPendingChanges(Context context)
{
    std::for_each(pendingList.begin(), pendingList.end(), [this, context] (PendingChange& change)
    {
        switch (change.action)
        {
        case Push:
            stack.push_back(createState(change.stateID));
            context.resourceManager->loadResources(stack.back()->getNeededResources());
            break;
        case Pop:
            stack.pop_back();
            context.resourceManager->unloadCheckResources();
            break;
        case Clear:
            stack.clear();
            context.resourceManager->unloadCheckResources();
            break;
        }
    } );
    pendingList.clear();
}
