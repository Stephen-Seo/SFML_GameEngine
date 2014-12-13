
#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <memory>
#include <map>

#include <SFML/System.hpp>

#include "node.hpp"

struct Context;

class System : private sf::NonCopyable
{
public:
    System(std::unique_ptr<Node> nodeType);

    bool checkEntity(Entity& entity);
    void removeEntity(int eID);
    void clearEntities();

    void update(sf::Time dt, Context context);
private:
    std::unique_ptr<Node> nodeType;
    std::map<int, std::unique_ptr<Node> > nodeMap;
};

#endif
