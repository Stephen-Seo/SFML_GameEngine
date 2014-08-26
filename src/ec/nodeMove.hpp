
#ifndef NODE_MOVE_HPP
#define NODE_MOVE_HPP

#include "node.hpp"
#include "componentPosition.hpp"
#include "componentVelocity.hpp"
#include "componentAcceleration.hpp"
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
