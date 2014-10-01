
#include "gui.hpp"

#include "utility.hpp"

GuiObject::GuiObject() :
transformDirty(true)
{
}

void GuiObject::registerCallback(std::function<void(float)> function)
{
    callbacks.push_back(function);
}

void GuiObject::clearCallbacks()
{
    callbacks.clear();
}

void GuiObject::setPosition(float x, float y)
{
    transformable.setPosition(x, y);
    transformDirty = true;
}

void GuiObject::setPosition(const sf::Vector2f& pos)
{
    transformable.setPosition(pos);
    transformDirty = true;
}

void GuiObject::setRotation(float angle)
{
    transformable.setRotation(angle);
    transformDirty = true;
}

void GuiObject::setScale(float factorX, float factorY)
{
    transformable.setScale(factorX, factorY);
    transformDirty = true;
}

void GuiObject::setScale(const sf::Vector2f& factors)
{
    transformable.setScale(factors);
    transformDirty = true;
}

void GuiObject::setOrigin(float x, float y)
{
    transformable.setOrigin(x, y);
    transformDirty = true;
}

void GuiObject::setOrigin(const sf::Vector2f& origin)
{
    transformable.setOrigin(origin);
    transformDirty = true;
}

const sf::Vector2f& GuiObject::getPosition() const
{
    return transformable.getPosition();
}

float GuiObject::getRotation() const
{
    return transformable.getRotation();
}

const sf::Vector2f& GuiObject::getScale() const
{
    return transformable.getScale();
}

const sf::Vector2f& GuiObject::getOrigin() const
{
    return transformable.getOrigin();
}

void GuiObject::move(float offsetX, float offsetY)
{
    transformable.move(offsetX, offsetY);
    transformDirty = true;
}

void GuiObject::move(const sf::Vector2f& offset)
{
    transformable.move(offset);
    transformDirty = true;
}

void GuiObject::rotate(float angle)
{
    transformable.rotate(angle);
    transformDirty = true;
}

void GuiObject::scale(float factorX, float factorY)
{
    transformable.scale(factorX, factorY);
    transformDirty = true;
}

void GuiObject::scale(const sf::Vector2f& factor)
{
    transformable.scale(factor);
    transformDirty = true;
}

const sf::Transform& GuiObject::getTransform() const
{
    return transformable.getTransform();
}

const sf::Transform& GuiObject::getInverseTransform() const
{
    return transformable.getInverseTransform();
}

GuiButton::GuiButton(bool usingTexture) :
GuiObject(),
usingTexture(usingTexture),
currentState(PASSIVE),
passiveTexture(nullptr),
hoveringTexture(nullptr),
activeTexture(nullptr)
{
    if(usingTexture)
    {
        passiveFillColor = sf::Color::Transparent;
        passiveOutlineColor = sf::Color::Transparent;

        hoveringFillColor = sf::Color::Transparent;
        hoveringOutlineColor = sf::Color::Transparent;

        activeFillColor = sf::Color::Transparent;
        activeOutlineColor = sf::Color::Transparent;
    }
    else
    {
        passiveFillColor = sf::Color::White;
        passiveOutlineColor = sf::Color::White;

        hoveringFillColor = sf::Color::White;
        hoveringOutlineColor = sf::Color::White;

        activeFillColor = sf::Color::White;
        activeOutlineColor = sf::Color::White;
    }

    rectangleShape.setFillColor(passiveFillColor);
    rectangleShape.setOutlineColor(passiveOutlineColor);
}

void GuiButton::update(sf::Time dt)
{}

void GuiButton::handleEvent(const sf::Event& event)
{
    if(event.type == sf::Event::MouseMoved)
    {
        if(transformDirty)
        {
            // get local coordinates of points
            coords[0] = getPosition();
            coords[2] = coords[0] + rectangleShape.getSize();
            coords[1] = coords[0];
            coords[1].x += rectangleShape.getSize().x;
            coords[3] = coords[0];
            coords[3].y += rectangleShape.getSize().y;

            // get global coordinates of points
            coords[0] = getTransform().transformPoint(coords[0]);
            coords[1] = getTransform().transformPoint(coords[1]);
            coords[2] = getTransform().transformPoint(coords[2]);
            coords[3] = getTransform().transformPoint(coords[3]);

            transformDirty = false;
        }

        // check if mouse point is within global bounds
        bool withinBounds = Utility::isWithinTransformedRectangle(coords, sf::Vector2f(event.mouseMove.x, event.mouseMove.y));

        if(withinBounds)
        {
            if(currentState == PASSIVE)
            {
                currentState = HOVERING;
                if(usingTexture)
                {
                    rectangleShape.setTexture(hoveringTexture);
                }
                else
                {
                    rectangleShape.setFillColor(hoveringFillColor);
                    rectangleShape.setOutlineColor(hoveringOutlineColor);
                }
            }
            else if(currentState == AWAY_CLICKED_ON)
            {
                currentState = ACTIVE;
                if(usingTexture)
                {
                    rectangleShape.setTexture(activeTexture);
                }
                else
                {
                    rectangleShape.setFillColor(activeFillColor);
                    rectangleShape.setOutlineColor(activeOutlineColor);
                }
            }
        }
        else
        {
            if(currentState == HOVERING)
            {
                currentState = PASSIVE;
                if(usingTexture)
                {
                    rectangleShape.setTexture(passiveTexture);
                }
                else
                {
                    rectangleShape.setFillColor(passiveFillColor);
                    rectangleShape.setOutlineColor(passiveOutlineColor);
                }
            }
            else if(currentState == ACTIVE)
            {
                currentState = AWAY_CLICKED_ON;
                if(usingTexture)
                {
                    rectangleShape.setTexture(passiveTexture);
                }
                else
                {
                    rectangleShape.setFillColor(passiveFillColor);
                    rectangleShape.setOutlineColor(passiveOutlineColor);
                }
            }
        }
    }
    else if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        if(currentState == HOVERING)
        {
            currentState = ACTIVE;
            if(usingTexture)
            {
                rectangleShape.setTexture(activeTexture);
            }
            else
            {
                rectangleShape.setFillColor(activeFillColor);
                rectangleShape.setOutlineColor(activeOutlineColor);
            }
        }
    }
    else if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
    {
        if(currentState == ACTIVE)
        {
            currentState = HOVERING;
            for(int i = 0; i < callbacks.size(); ++i)
                callbacks[i](1.0f);
            if(usingTexture)
            {
                rectangleShape.setTexture(hoveringTexture);
            }
            else
            {
                rectangleShape.setFillColor(hoveringFillColor);
                rectangleShape.setOutlineColor(hoveringOutlineColor);
            }
        }
        else if(currentState == AWAY_CLICKED_ON)
        {
            currentState = PASSIVE;
            if(usingTexture)
            {
                rectangleShape.setTexture(passiveTexture);
            }
            else
            {
                rectangleShape.setFillColor(passiveFillColor);
                rectangleShape.setOutlineColor(passiveOutlineColor);
            }
        }
    }
}

void GuiButton::setSize(sf::Vector2f size)
{
    rectangleShape.setSize(size);
}

void GuiButton::setPassiveFillColor(sf::Color color)
{
    passiveFillColor = color;
}

void GuiButton::setPassiveOutlineColor(sf::Color color)
{
    passiveOutlineColor = color;
}

void GuiButton::setHoveringFillColor(sf::Color color)
{
    hoveringFillColor = color;
}

void GuiButton::setHoveringOutlineColor(sf::Color color)
{
    hoveringOutlineColor = color;
}

void GuiButton::setActiveFillColor(sf::Color color)
{
    activeFillColor = color;
}

void GuiButton::setActiveOutlineColor(sf::Color color)
{
    activeOutlineColor = color;
}

void GuiButton::setPassiveTexture(sf::Texture& texture)
{
    passiveTexture = &texture;
}

void GuiButton::setHoveringTexture(sf::Texture& texture)
{
    hoveringTexture = &texture;
}

void GuiButton::setActiveTexture(sf::Texture& texture)
{
    activeTexture = &texture;
}

void GuiButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(rectangleShape, states);
}
