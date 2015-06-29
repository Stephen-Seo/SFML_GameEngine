
#include "stateStack.hpp"

#include <typeinfo>

#include "resourceManager.hpp"

StateStack::PendingChange::PendingChange(Action action, std::string stateID)
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

void StateStack::pushState(const std::string& stateName)
{
    pendingList.push_back(PendingChange(StateStack::Push, stateName));
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

std::vector<std::size_t> StateStack::getContentsHashCodes()
{
    std::vector<std::size_t> hashCodes;

    for(auto stateIter = stack.begin(); stateIter != stack.end(); ++stateIter)
    {
        hashCodes.push_back(typeid(stateIter->get()).hash_code());
    }

    return hashCodes;
}

State::Ptr StateStack::createState(const std::string& stateID)
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
            context.resourceManager->loadResources();
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
