
#include "entity.hpp"

#include <utility>

int Entity::gID = 0;

std::set<int> Entity::IDsInUse;

Entity::Entity() :
removed(false)
{
    ID = Entity::gID++;
    while(!Entity::IDsInUse.insert(ID).second)
    {
        ID = Entity::gID++;
    }
}

Entity::~Entity()
{
    Entity::IDsInUse.erase(ID);
}

void Entity::addComponent(std::type_index typeIndex, std::unique_ptr<Component> component)
{
    cMap.insert(std::make_pair(typeIndex, std::move(component)));
}

bool Entity::removeComponent(std::type_index typeIndex)
{
    return cMap.erase(typeIndex) == 1;
}

Component* Entity::getComponent(std::type_index typeIndex)
{
    auto iter = cMap.find(typeIndex);
    if(iter != cMap.end())
    {
        return iter->second.get();
    }

    return nullptr;
}

std::list<Component*> Entity::getAllComponents()
{
    std::list<Component*> list;
    for(auto iter = cMap.begin(); iter != cMap.end(); ++iter)
    {
        list.push_back(iter->second.get());
    }

    return list;
}

bool Entity::hasComponent(std::type_index typeIndex)
{
    return cMap.find(typeIndex) != cMap.end();
}

int Entity::getID()
{
    return ID;
}

