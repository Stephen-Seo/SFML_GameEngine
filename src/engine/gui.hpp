
#ifndef GAME_GUI_HPP
#define GAME_GUI_HPP

#include <functional>
#include <vector>

#include <SFML/Graphics.hpp>

class GuiObject : public sf::Drawable
{
public:
    GuiObject();

    virtual void update(sf::Time dt) = 0;
    virtual void handleEvent(const sf::Event& event) = 0;

    void registerCallback(std::function<void(float)> function);
    void clearCallbacks();

    void setPosition(float x, float y);
    void setPosition(const sf::Vector2f& pos);
    void setRotation(float angle);
    void setScale(float factorX, float factorY);
    void setScale(const sf::Vector2f& factors);
    void setOrigin(float x, float y);
    void setOrigin(const sf::Vector2f& origin);

    const sf::Vector2f& getPosition() const;
    float getRotation() const;
    const sf::Vector2f& getScale() const;
    const sf::Vector2f& getOrigin() const;

    void move(float offsetX, float offsetY);
    void move(const sf::Vector2f& offset);
    void rotate(float angle);
    void scale(float factorX, float factorY);
    void scale(const sf::Vector2f& factor);

    const sf::Transform& getTransform() const;
    const sf::Transform& getInverseTransform() const;

protected:
    std::vector<std::function<void(float)> > callbacks;
    bool transformDirty;

private:
    sf::Transformable transformable;

};

class GuiButton : public GuiObject
{
public:
    GuiButton(bool usingTexture = false);

    void update(sf::Time dt);
    void handleEvent(const sf::Event& event);

    void setSize(const sf::Vector2f& size);
    const sf::Vector2f& getSize();

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

    sf::Vector2f coords[4];

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class GuiSlider : public GuiObject
{
public:
    GuiSlider(bool usingTexture = false);

    void update(sf::Time dt);
    void handleEvent(const sf::Event& event);

    void setBgSize(const sf::Vector2f& size);
    void setKnobSize(const sf::Vector2f& size);

    void setBgFillColor(sf::Color);
    void setBgOutlineColor(sf::Color);

    void setKnobFillColor(sf::Color);
    void setKnobOutlineColor(sf::Color);

    void setBgTexture(sf::Texture& texture);
    void setKnobTexture(sf::Texture& texture);

private:
    sf::Uint8 status;
    /*
        0000 0000 - passive
        0000 0001 - active
        1000 0000 - using texture
    */

    sf::RectangleShape bg;
    sf::RectangleShape knob;

    float knobLocation;

    sf::Vector2f coords[4];

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    void centerKnobOnBg(float location);
};

#endif
