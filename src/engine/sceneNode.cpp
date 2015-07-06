
#include "sceneNode.hpp"

SceneNode::SceneNode()
{
    parent = nullptr;
}

SceneNode::~SceneNode()
{}

void SceneNode::attachChild(SceneNode::Ptr child)
{
    child->parent = this;
    attachRequests.push_back(std::move(child));
}

void SceneNode::attachChildFront(SceneNode::Ptr child)
{
    child->parent = this;
    attachRequestsFront.push_back(std::move(child));
}

void SceneNode::attachToRoot(SceneNode::Ptr child)
{
    if(parent != nullptr)
    {
        parent->attachToRoot(std::move(child));
    }
    else
    {
        child->parent = this;
        attachRequests.push_back(std::move(child));
    }
}

void SceneNode::attachToRootFront(SceneNode::Ptr child)
{
    if(parent != nullptr)
    {
        parent->attachToRootFront(std::move(child));
    }
    else
    {
        child->parent = this;
        attachRequestsFront.push_back(std::move(child));
    }
}

void SceneNode::detachChild(SceneNode* node)
{
    detachRequests.push_back(node);
}

void SceneNode::clear()
{
    children.clear();
}

void SceneNode::draw(sf::RenderTarget& target,
                     sf::RenderStates states) const
{
    states.transform *= getTransform();

    drawCurrent(target, states);
    drawChildren(target, states);
}

void SceneNode::drawCurrent(sf::RenderTarget& target,
                            sf::RenderStates states) const
{}

void SceneNode::drawChildren(sf::RenderTarget& target,
                             sf::RenderStates states) const
{
    std::for_each(children.begin(), children.end(),
        [&target, &states] (const SceneNode::Ptr& child) { child->draw(target, states); });
}

void SceneNode::update(sf::Time dt, Context context)
{
    updateCurrent(dt, context);
    updateChildren(dt, context);

    attachChildren();
    detachChildren();
}

void SceneNode::updateCurrent(sf::Time, Context context)
{}

void SceneNode::updateChildren(sf::Time dt, Context context)
{
    std::for_each(children.begin(), children.end(),
        [&dt, &context] (SceneNode::Ptr& child) { child->update(dt, context); });
}

void SceneNode::handleEvent(const sf::Event& event, Context context)
{
    handleEventCurrent(event, context);
    passEvent(event, context);

    attachChildren();
    detachChildren();
}

void SceneNode::handleEventCurrent(const sf::Event& event, Context context)
{}

void SceneNode::passEvent(const sf::Event& event, Context context)
{
    std::for_each(children.begin(), children.end(),
        [&event, &context] (const SceneNode::Ptr& child) { child->handleEvent(event, context); });
}

sf::Transform SceneNode::getWorldTransform() const
{
    sf::Transform transform = sf::Transform::Identity;
    for (const SceneNode* node = this; node != nullptr; node = node->parent)
        transform = node->getTransform() * transform;

    return transform;
}

sf::Vector2f SceneNode::getWorldPosition() const
{
    return getWorldTransform() * sf::Vector2f();
}

void SceneNode::forEach(std::function<void(SceneNode&)> function, bool includeThis, unsigned int maxDepth)
{
    forEach(function, includeThis, maxDepth, 0U);
}

void SceneNode::forEach(std::function<void(SceneNode&)> function, bool includeThis, unsigned int maxDepth, unsigned int currentDepth)
{
    if(includeThis)
    {
        function(*this);
    }

    if(maxDepth == 0U || currentDepth + 1 <= maxDepth)
    {
        for(auto node = children.begin(); node != children.end(); ++node)
        {
            (*node)->forEach(function, true, maxDepth, currentDepth + 1);
        }
    }
}

bool SceneNode::operator ==(const SceneNode& other) const
{
    return this == &other;
}

void SceneNode::detachSelf()
{
    if(parent != nullptr)
    {
        parent->detachChild(this);
    }
}

void SceneNode::attachChildren()
{
    while(!attachRequests.empty())
    {
        children.push_back(std::move(attachRequests.back()));
        attachRequests.pop_back();
    }
    while(!attachRequestsFront.empty())
    {
        children.push_front(std::move(attachRequestsFront.back()));
        attachRequestsFront.pop_back();
    }
}

void SceneNode::detachChildren()
{
    while(!detachRequests.empty())
    {
        SceneNode* ptr = detachRequests.back();
        auto found = std::find_if(children.begin(), children.end(), [&ptr] (const SceneNode::Ptr& pointer) {
            return pointer.get() == ptr;
        });

        assert(found != children.end());

        children.erase(found);
        detachRequests.pop_back();
    }
}

