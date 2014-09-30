
#ifndef GAME_GUI_HPP
#define GAME_GUI_HPP

#include <functional>
#include <vector>

#include <SFML/Graphics.hpp>

class GuiObject : public sf::Drawable, public sf::Transformable
{
public:
    virtual void update(sf::Time dt) = 0;
    virtual void handleEvent(const sf::Event& event) = 0;

    void registerCallback(std::function<void(float)> function);

protected:
    std::vector<std::function<void(float)> > callbacks;

};

class GuiButton : public GuiObject
{
public:
    GuiButton(bool usingTexture = false);

    void update(sf::Time dt);
    void handleEvent(const sf::Event& event);

    void setSize(sf::Vector2f size);

    void setPassiveFillColor(sf::Color color);
    void setPassiveOutlineColor(sf::Color color);

    void setHoveringFillColor(sf::Color color);
    void setHoveringOutlineColor(sf::Color color);

    void setActiveFillColor(sf::Color color);
    void setActiveOutlineColor(sf::Color color);

    void setPassiveTexture(sf::Texture& texture);
    void setHoveringTexture(sf::Texture& texture);
    void setActiveTexture(sf::Texture& texture);

private:
    enum MouseState
    {
        PASSIVE,
        HOVERING,
        ACTIVE,
        AWAY_CLICKED_ON
    };

    bool usingTexture;
    MouseState currentState;

    sf::Color passiveFillColor;
    sf::Color passiveOutlineColor;

    sf::Color hoveringFillColor;
    sf::Color hoveringOutlineColor;

    sf::Color activeFillColor;
    sf::Color activeOutlineColor;

    sf::Texture* passiveTexture;
    sf::Texture* hoveringTexture;
    sf::Texture* activeTexture;

    sf::RectangleShape rectangleShape;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif
