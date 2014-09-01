
#include "engine.hpp"

#include <stdexcept>

#include "../context.hpp"

Engine::Engine()
{
}

void Engine::addSystem(std::unique_ptr<Node> type)
{
    System* newSystem = new System(std::move(type));
    systems.push_back(std::unique_ptr<System>(newSystem));
}

void Engine::addDrawSystem(std::unique_ptr<Node> type)
{
    System* newSystem = new System(std::move(type));
    drawSystems.push_back(std::unique_ptr<System>(newSystem));
}

void Engine::addEntity(std::unique_ptr<Entity> entity)
{
    entity->removed = false;
    bool nodeAdded = false;
    for(auto iter = systems.begin(); iter != systems.end(); ++iter)
    {
        nodeAdded |= (*iter)->checkEntity(*entity.get());
    }
    for(auto iter = drawSystems.begin(); iter != drawSystems.end(); ++iter)
    {
        nodeAdded |= (*iter)->checkEntity(*entity.get());
    }
    if(!nodeAdded)
        throw new std::runtime_error("No new nodes added with entity (entity cannot possibly be updated)!");
    entityMap.insert(std::make_pair(entity->getID(), std::move(entity)));
}

void Engine::removeEntity(int eID)
{
    auto iter = entityMap.find(eID);
    if(iter != entityMap.end())
    {
        iter->second->removed = true;
        deadQueue.push(eID);
    }
}

std::map<int, std::unique_ptr<Entity> >::iterator Engine::getEntityIterator()
{
    return entityMap.begin();
}

void Engine::update(sf::Time dt, Context context)
{
    while(!deadQueue.empty())
    {
        int eID = deadQueue.front();
        deadQueue.pop();

        for(auto iter = systems.begin(); iter != systems.end(); ++iter)
        {
            (*iter)->removeEntity(eID);
        }
        for(auto iter = drawSystems.begin(); iter != drawSystems.end(); ++iter)
        {
            (*iter)->removeEntity(eID);
        }

        entityMap.erase(eID);
    }

    for(auto iter = systems.begin(); iter != systems.end(); ++iter)
    {
        (*iter)->update(dt, context);
    }
}

void Engine::draw(Context context)
{
    for(auto iter = drawSystems.begin(); iter != drawSystems.end(); ++iter)
    {
        (*iter)->update(sf::Time::Zero, context);
    }
}
