
#ifndef NODE_HPP
#define NODE_HPP

#include <memory>

#include <SFML/System.hpp>

#include "entity.hpp"

struct Context;

struct Node
{
public:
    virtual ~Node() {}

    virtual bool checkEntity(Entity& entity) = 0;
    virtual std::unique_ptr<Node> getNewNode() = 0;
    virtual void getCReferencesFromEntity(Entity& entity) = 0;
    virtual void update(sf::Time dt, Context context) = 0;
protected:
    bool* entityRemoved;
};

#endif
