
#include "gui.hpp"

#include "utility.hpp"

void GuiObject::registerCallback(std::function<void(float)> function)
{
    callbacks.push_back(function);
}

GuiButton::GuiButton(bool usingTexture) :
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
        // get local coordinates of points
        sf::Vector2f coords[4];
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

        // check if mouse point is within global bounds
        bool withinBounds = Utility::isWithinTransformedRectangle(coords, sf::Vector2f(event.mouseMove.x, event.mouseMove.y));

        if(withinBounds)
        {
            if(currentState == PASSIVE)
            {
                currentState = HOVERING;
                if(usingTexture)
                {
                    rectangleShape.setTexture(*hoveringTexture);
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
                    rectangleShape.setTexture(*activeTexture);
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
                rectangleShape.setTexture(*activeTexture);
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
                rectangleShape.setTexture(*hoveringTexture);
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

// TODO the rest of GuiButton functions, and possibly make sf::Transformable a variable instead of
// something to derive from for better control for the sake of caching positions

void GuiButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(rectangleShape, states);
}
