
#ifndef SCENE_NODE_HPP
#define SCENE_NODE_HPP

#include <memory>
#include <algorithm>
#include <cassert>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class SceneNode : public sf::Transformable, public sf::Drawable,
                  private sf::NonCopyable
{
public:
    typedef std::unique_ptr<SceneNode> Ptr;

    SceneNode();

    void attachChild(Ptr child);
    Ptr detachChild(const SceneNode& node);

    void update(sf::Time dt);

    sf::Transform getWorldTransform() const;
    sf::Vector2f getWorldPosition() const;
private:
    virtual void draw(sf::RenderTarget& target,
                      sf::RenderStates states) const;
    virtual void drawCurrent(sf::RenderTarget& target,
                             sf::RenderStates states) const;

    virtual void updateCurrent(sf::Time dt);
    void updateChildren(sf::Time dt);

    std::vector<Ptr>    children;
    SceneNode*          parent;
};

#endif
