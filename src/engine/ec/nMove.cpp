
#include "nMove.hpp"

#include <typeindex>

#include "../context.hpp"

nMove::nMove() :
position(nullptr),
velocity(nullptr),
acceleration(nullptr)
{}

bool nMove::checkEntity(Entity& entity)
{
    return entity.hasComponent(std::type_index(typeid(cPosition))) &&
           entity.hasComponent(std::type_index(typeid(cVelocity))) &&
           entity.hasComponent(std::type_index(typeid(cAcceleration)));
}

std::unique_ptr<Node> nMove::getNewNode()
{
    return std::unique_ptr<Node>(new nMove());
}

void nMove::getCReferencesFromEntity(Entity& entity)
{
    position = static_cast<cPosition*>(entity.getComponent(std::type_index(typeid(cPosition))));
    velocity = static_cast<cVelocity*>(entity.getComponent(std::type_index(typeid(cVelocity))));
    acceleration = static_cast<cAcceleration*>(entity.getComponent(std::type_index(typeid(cAcceleration))));
    entityRemoved = &entity.removed;
}

void nMove::update(sf::Time dt, Context context)
{
    if(!*entityRemoved)
    {
        velocity->x += acceleration->x * dt.asSeconds();
        velocity->y += acceleration->y * dt.asSeconds();
        velocity->rot += acceleration->rot * dt.asSeconds();

        position->x += velocity->x * dt.asSeconds();
        position->y += velocity->y * dt.asSeconds();
        position->rot += velocity->rot * dt.asSeconds();
    }
}
