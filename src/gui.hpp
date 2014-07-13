
#ifndef GAME_GUI_HPP
#define GAME_GUI_HPP

#include <list>
#include <memory>
#include <functional>
#include <cassert>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "state.hpp"

class GuiCommand
{
public:
    enum Type {
        VALUE_BOOL,
        VALUE_INT,
        VALUE_FLOAT,
        STATE
    };

    enum State_Type {
        POP,
        PUSH,
        POP_THEN_PUSH,
        CLEAR_THEN_PUSH
    };

    union Value {
        Value(bool b);
        Value(int i);
        Value(float f);
        Value(States::ID id);

        bool b;
        int i;
        float f;
        States::ID id;
    };

    union Ptr {
        Ptr(bool* b);
        Ptr(int* i);
        Ptr(float* f);
        Ptr(State_Type s);
        Ptr();

        bool* b;
        int* i;
        float* f;
        State_Type s;
    };

    GuiCommand(Type type, Value value, Ptr ptr);

    Type type;
    Value value;

private:
    friend class GuiSystem;
    Ptr ptr;
};

class GuiObject : public sf::Drawable, public sf::Transformable
{
public:
    typedef std::unique_ptr<GuiObject> Ptr;

    GuiObject(sf::RenderWindow* window, GuiCommand guiCommand);
    virtual ~GuiObject();

    virtual void processEvent(const sf::Event& event) = 0;
    virtual GuiCommand* update(sf::Time time) = 0;
protected:
    bool passCommand;
    GuiCommand guiCommand;
    sf::RenderWindow* window;
};

class GuiButton : public GuiObject
{
public:
    typedef std::unique_ptr<GuiButton> Ptr;

    GuiButton(sf::RenderWindow* window, GuiCommand guiCommand, sf::Color color, sf::Color activeColor, sf::Vector2f size, sf::Text text = sf::Text());
    GuiButton(sf::RenderWindow* window, GuiCommand guiCommand, const sf::Texture& texture, const sf::Texture& hovering, const sf::Texture& active, sf::Text text = sf::Text());

    virtual void processEvent(const sf::Event& event);
    virtual GuiCommand* update(sf::Time time);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

protected:
    bool usesTexture;
    sf::Text text;
    sf::Sprite normalSprite;
    sf::Sprite hoveringSprite;
    sf::Sprite activeSprite;
    sf::Color color;
    sf::Color activeColor;
    sf::RectangleShape rect;

private:
    bool hovering;
    bool clickedOn;
};

class GuiSlider : public GuiObject
{
public:
    typedef std::unique_ptr<GuiSlider> Ptr;

    GuiSlider(sf::RenderWindow* window, GuiCommand guiCommand, float currentValue, float llimit, float hlimit, sf::Color slider, sf::Color bg, sf::Vector2f sliderSize, sf::FloatRect sliderRect);
    GuiSlider(sf::RenderWindow* window, GuiCommand guiCommand, float currentValue, float llimit, float hlimit, const sf::Texture& slider, const sf::Texture& bg);

    virtual void processEvent(const sf::Event& event);
    virtual GuiCommand* update(sf::Time time);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

protected:
    bool usesTexture;

    sf::Sprite sliderSprite;
    sf::Sprite bgSprite;
    sf::RectangleShape sliderShape;
    sf::RectangleShape bgShape;
    sf::FloatRect rect;
    float currentValue;
    float llimit;
    float hlimit;
    float offset;
    bool clickedOn;

    virtual GuiCommand* returnValue();
};

class GuiIntSlider : public GuiSlider
{
public:
    GuiIntSlider(sf::RenderWindow* window, GuiCommand guiCommand, float currentValue, float llimit, float hlimit, sf::Color slider, sf::Color bg, sf::Vector2f sliderSize, sf::FloatRect sliderRect);
    GuiIntSlider(sf::RenderWindow* window, GuiCommand guiCommand, float currentValue, float llimit, float hlimit, const sf::Texture& slider, const sf::Texture& bg);

protected:
    GuiCommand* returnValue();

};

class GuiCheckbox : public GuiObject
{
public:
    typedef std::unique_ptr<GuiCheckbox> Ptr;

    GuiCheckbox(sf::RenderWindow* window, GuiCommand guiCommand, bool initialValue, sf::Color color, sf::Vector2f size);
    GuiCheckbox(sf::RenderWindow* window, GuiCommand guiCommand, bool initialValue, const sf::Texture& box, const sf::Texture& check);

    virtual void processEvent(const sf::Event& event);
    virtual GuiCommand* update(sf::Time time);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
protected:
    bool usesTexture;
    bool currentValue;
    bool clickedOn;

    sf::Sprite boxSprite;
    sf::Sprite checkSprite;
    sf::RectangleShape boxShape;
    sf::VertexArray checkLines;
};

class GuiText : public GuiObject
{
public:
    typedef std::unique_ptr<GuiText> Ptr;

    GuiText(sf::RenderWindow* window, GuiCommand guiCommand, sf::Text* text);

    virtual void processEvent(const sf::Event& event);
    virtual GuiCommand* update(sf::Time time);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
protected:
    sf::Text* text;
};

class GuiImage : public GuiObject
{
public:
    typedef std::unique_ptr<GuiImage> Ptr;

    GuiImage(sf::RenderWindow* window, GuiCommand guiCommand, const sf::Texture& texture);

    virtual void processEvent(const sf::Event& event);
    virtual GuiCommand* update(sf::Time time);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
protected:
    sf::Sprite sprite;
};

class GuiSystem : private sf::NonCopyable
{
public:
    GuiSystem(State* state);

    void processEvent(const sf::Event& event);
    void update(sf::Time time);
    void draw(sf::RenderWindow& window);

    void add(GuiObject* guiObject);
    void add(GuiObject::Ptr& guiObject);
    void clear();
private:
    std::list<GuiObject::Ptr> guiList;
    State* state;
};

#endif
