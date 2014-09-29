
#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <vector>
#include <queue>
#include <memory>
#include <functional>

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
    void clear();
    void clearEntities();
    std::map<int, std::unique_ptr<Entity> >::iterator getEntityIterBegin();
    std::map<int, std::unique_ptr<Entity> >::iterator getEntityIterEnd();

    void update(sf::Time dt, Context context);
    void draw(Context context);
    void registerRemoveCall(int eID, std::function<void()> function);
private:
    std::vector<std::unique_ptr<System> > systems;
    std::vector<std::unique_ptr<System> > drawSystems;
    std::map<int, std::unique_ptr<Entity> > entityMap;
    std::queue<int> deadQueue;
    std::map<int, std::queue<std::function<void()> > > rfMap;
};

#endif
