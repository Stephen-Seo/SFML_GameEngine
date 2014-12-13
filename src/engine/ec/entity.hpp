
#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <typeindex>
#include <memory>
#include <map>
#include <list>
#include <set>

#include <SFML/System.hpp>

#include "component.hpp"

class Entity : private sf::NonCopyable
{
public:
    Entity();
    ~Entity();

    bool removed;

    void addComponent(std::type_index typeIndex, std::unique_ptr<Component> component);
    bool removeComponent(std::type_index typeIndex);
    Component* getComponent(std::type_index typeIndex);
    std::list<Component*> getAllComponents();
    bool hasComponent(std::type_index typeIndex);
    int getID();
private:
    static int gID;
    static std::set<int> IDsInUse;
    int ID;

    std::map<std::type_index, std::unique_ptr<Component> > cMap;
};

#endif
