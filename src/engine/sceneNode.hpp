
#ifndef SCENE_NODE_HPP
#define SCENE_NODE_HPP

#include <memory>
#include <algorithm>
#include <cassert>
#include <functional>
#include <vector>
#include <list>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "context.hpp"

#ifndef NDEBUG
  #include <iostream>
#endif

class SceneNode : public sf::Transformable, public sf::Drawable,
                  private sf::NonCopyable
{
public:
    typedef std::unique_ptr<SceneNode> Ptr;

    SceneNode();
    virtual ~SceneNode();

    void attachChild(Ptr child);
    void attachChildFront(Ptr child);
    void attachToRoot(Ptr child);
    void attachToRootFront(Ptr child);
    void detachChild(SceneNode* node);
    void clear();

    void update(sf::Time dt, Context context);
    void handleEvent(const sf::Event& event, Context context);

    sf::Transform getWorldTransform() const;
    sf::Vector2f getWorldPosition() const;

    void forEach(std::function<void(SceneNode&)> function, bool includeThis = false, unsigned int maxDepth = 0U);

    bool operator ==(const SceneNode& other) const;

protected:
    void detachSelf();

private:
    virtual void draw(sf::RenderTarget& target,
                      sf::RenderStates states) const;
    virtual void drawCurrent(sf::RenderTarget& target,
                             sf::RenderStates states) const;
    void drawChildren(sf::RenderTarget& target,
                      sf::RenderStates states) const;

    virtual void updateCurrent(sf::Time dt, Context context);
    void updateChildren(sf::Time dt, Context context);

    virtual void handleEventCurrent(const sf::Event& event, Context context);
    void passEvent(const sf::Event& event, Context context);

    void forEach(std::function<void(SceneNode&)> function, bool includeThis, unsigned int maxDepth, unsigned int currentDepth);

    void attachChildren();
    void detachChildren();

    std::list<Ptr>    children;
    std::vector<SceneNode::Ptr> attachRequests;
    std::vector<SceneNode::Ptr> attachRequestsFront;
    std::vector<SceneNode*> detachRequests;
    SceneNode*          parent;

};

#endif

