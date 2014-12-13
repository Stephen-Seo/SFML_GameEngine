
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

        rectangleShape.setFillColor(sf::Color::Transparent);
        rectangleShape.setOutlineColor(sf::Color::Transparent);
    }
    else
    {
        passiveFillColor = sf::Color::White;
        passiveOutlineColor = sf::Color::Black;

        hoveringFillColor = sf::Color::White;
        hoveringOutlineColor = sf::Color::White;

        activeFillColor = sf::Color::Black;
        activeOutlineColor = sf::Color::White;

        rectangleShape.setFillColor(sf::Color::White);
        rectangleShape.setOutlineColor(sf::Color::Black);
    }

    rectangleShape.setSize(sf::Vector2f(16.0f, 16.0f));
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
            coords[0].x = 0.0f;
            coords[0].y = 0.0f;
            coords[2] = rectangleShape.getSize();
            coords[1].x = coords[2].x;
            coords[1].y = 0.0f;
            coords[3].x = 0.0f;
            coords[3].y = coords[2].y;

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

void GuiButton::setSize(const sf::Vector2f& size)
{
    rectangleShape.setSize(size);
}

const sf::Vector2f& GuiButton::getSize()
{
    return rectangleShape.getSize();
}

void GuiButton::setPassiveFillColor(sf::Color color)
{
    passiveFillColor = color;
    if(currentState == PASSIVE && !usingTexture)
        rectangleShape.setFillColor(color);
}

void GuiButton::setPassiveOutlineColor(sf::Color color)
{
    passiveOutlineColor = color;
    if(currentState == PASSIVE && !usingTexture)
        rectangleShape.setOutlineColor(color);
}

void GuiButton::setHoveringFillColor(sf::Color color)
{
    hoveringFillColor = color;
    if(currentState == HOVERING && !usingTexture)
        rectangleShape.setFillColor(color);
}

void GuiButton::setHoveringOutlineColor(sf::Color color)
{
    hoveringOutlineColor = color;
    if(currentState == HOVERING && !usingTexture)
        rectangleShape.setOutlineColor(color);
}

void GuiButton::setActiveFillColor(sf::Color color)
{
    activeFillColor = color;
    if(currentState == ACTIVE && !usingTexture)
        rectangleShape.setFillColor(color);
}

void GuiButton::setActiveOutlineColor(sf::Color color)
{
    activeOutlineColor = color;
    if(currentState == ACTIVE && !usingTexture)
        rectangleShape.setOutlineColor(color);
}

void GuiButton::setPassiveTexture(sf::Texture& texture)
{
    passiveTexture = &texture;
    if(currentState == PASSIVE && usingTexture)
        rectangleShape.setTexture(&texture);
}

void GuiButton::setHoveringTexture(sf::Texture& texture)
{
    hoveringTexture = &texture;
    if(currentState == HOVERING && usingTexture)
        rectangleShape.setTexture(&texture);
}

void GuiButton::setActiveTexture(sf::Texture& texture)
{
    activeTexture = &texture;
    if(currentState == ACTIVE && usingTexture)
        rectangleShape.setTexture(&texture);
}

void GuiButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(rectangleShape, states);
}

GuiSlider::GuiSlider(bool usingTexture) :
GuiObject(),
status(usingTexture ? 0x80 : 0x0),
knobLocation(0.0f)
{
    if(usingTexture)
    {
        bg.setFillColor(sf::Color::Transparent);
        bg.setOutlineColor(sf::Color::Transparent);
        knob.setFillColor(sf::Color::Transparent);
        knob.setOutlineColor(sf::Color::Transparent);
    }
    else
    {
        bg.setFillColor(sf::Color::White);
        bg.setOutlineColor(sf::Color::Black);

        knob.setFillColor(sf::Color::White);
        knob.setOutlineColor(sf::Color::Black);
    }

    bg.setSize(sf::Vector2f(64.0f, 16.0f));
    knob.setSize(sf::Vector2f(16.0f, 16.0f));
    centerKnobOnBg(0.0f);
}

void GuiSlider::update(sf::Time dt)
{
}

void GuiSlider::handleEvent(const sf::Event& event)
{
    if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        if(transformDirty)
        {
            // get local coordinates of points
            sf::FloatRect rect = knob.getGlobalBounds();
            coords[0].x = rect.left;
            coords[0].y = rect.top;
            coords[1].x = rect.left + rect.width;
            coords[1].y = rect.top;
            coords[2].x = coords[1].x;
            coords[2].y = rect.top + rect.height;
            coords[3].x = rect.left;
            coords[3].y = coords[2].y;

            // get global coordinates of points
            coords[0] = getTransform().transformPoint(coords[0]);
            coords[1] = getTransform().transformPoint(coords[1]);
            coords[2] = getTransform().transformPoint(coords[2]);
            coords[3] = getTransform().transformPoint(coords[3]);

            transformDirty = false;           
        }

        bool withinBounds = Utility::isWithinTransformedRectangle(coords, sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
        if(withinBounds)
        {
            status |= 0x1;
        }
    }
    else if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
    {
        if((status & 0x1) != 0)
        {
            for(int i = 0; i < callbacks.size(); ++i)
                callbacks[i](knobLocation);
            status &= 0xFE;
        }
    }
    else if(event.type == sf::Event::MouseMoved && (status & 0x1) != 0x0)
    {
        sf::Vector2f point(event.mouseMove.x, event.mouseMove.y);
        point = getInverseTransform().transformPoint(point);
        float width = bg.getSize().x;

        if(point.x > 0.0f && point.x < width)
        {
            knobLocation = point.x / width;
        }
        else if(point.x <= 0.0f)
        {
            knobLocation = 0.0f;
        }
        else if(point.x >= width)
        {
            knobLocation = 1.0f;
        }
        centerKnobOnBg(knobLocation);

        for(int i = 0; i < callbacks.size(); ++i)
            callbacks[i](knobLocation);
    }
}

void GuiSlider::setBgSize(const sf::Vector2f& size)
{
    bg.setSize(size);
    centerKnobOnBg(knobLocation);
}

void GuiSlider::setKnobSize(const sf::Vector2f& size)
{
    knob.setSize(size);
    centerKnobOnBg(knobLocation);
}

void GuiSlider::setBgFillColor(sf::Color color)
{
    if((status & 0x80) == 0)
        bg.setFillColor(color);
}

void GuiSlider::setBgOutlineColor(sf::Color color)
{
    if((status & 0x80) == 0)
        bg.setOutlineColor(color);
}

void GuiSlider::setKnobFillColor(sf::Color color)
{
    if((status & 0x80) == 0)
        knob.setFillColor(color);
}

void GuiSlider::setKnobOutlineColor(sf::Color color)
{
    if((status & 0x80) == 0)
        knob.setOutlineColor(color);
}

void GuiSlider::setBgTexture(sf::Texture& texture)
{
    if((status & 0x80) != 0)
        bg.setTexture(&texture);
}

void GuiSlider::setKnobTexture(sf::Texture& texture)
{
    if((status & 0x80) != 0)
        knob.setTexture(&texture);
}

void GuiSlider::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(bg, states);
    target.draw(knob, states);
}

void GuiSlider::centerKnobOnBg(float location)
{
    sf::FloatRect rect = knob.getLocalBounds();
    knob.setOrigin(rect.left / 2.0f, rect.top / 2.0f);

    rect = bg.getLocalBounds();
    knob.setPosition(location * rect.width, rect.top / 2.0f);

    transformDirty = true;
}
