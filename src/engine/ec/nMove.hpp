
#ifndef NODE_MOVE_HPP
#define NODE_MOVE_HPP

#include "node.hpp"
#include "cPosition.hpp"
#include "cVelocity.hpp"
#include "cAcceleration.hpp"
#include "entity.hpp"

struct nMove : Node
{
    nMove();

    cPosition* position;
    cVelocity* velocity;
    cAcceleration* acceleration;

    bool checkEntity(Entity& entity);
    std::unique_ptr<Node> getNewNode();
    void getCReferencesFromEntity(Entity& entity);
    void update(sf::Time dt, Context context);
};

#endif
