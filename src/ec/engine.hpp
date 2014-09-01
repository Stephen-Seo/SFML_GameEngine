
#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <vector>
#include <queue>
#include <memory>

#include "system.hpp"

struct Context;

class Engine
{
public:
    Engine();
    void addSystem(std::unique_ptr<Node> type);
    void addDrawSystem(std::unique_ptr<Node> type);
    void addEntity(std::unique_ptr<Entity> entity);
    void removeEntity(int eID);
    std::map<int, std::unique_ptr<Entity> >::iterator getEntityIterator();

    void update(sf::Time dt, Context context);
    void draw(Context context);
private:
    std::vector<std::unique_ptr<System> > systems;
    std::vector<std::unique_ptr<System> > drawSystems;
    std::map<int, std::unique_ptr<Entity> > entityMap;
    std::queue<int> deadQueue;
};

#endif
