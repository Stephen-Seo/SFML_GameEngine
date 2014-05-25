
#include "gui.hpp"

GuiCommand::Value::Value(bool b) :
b(b)
{}

GuiCommand::Value::Value(int i) :
i(i)
{}

GuiCommand::Value::Value(float f) :
f(f)
{}

GuiCommand::Value::Value(States::ID id) :
id(id)
{}

GuiCommand::GuiCommand(GuiCommand::Type type, GuiCommand::Value value) :
type(type),
value(value)
{}

GuiObject::GuiObject(sf::RenderWindow* window, GuiCommand guiCommand) :
passCommand(false),
guiCommand(guiCommand),
window(window)
{}

GuiButton::GuiButton(sf::RenderWindow* window, GuiCommand guiCommand, sf::Color color, sf::Color activeColor, sf::Vector2f size, sf::Text text) :
GuiObject(window, guiCommand),
usesTexture(false),
text(text),
color(color),
activeColor(activeColor),
hovering(false),
clickedOn(false)
{
    this->rect.setSize(size);
    this->rect.setFillColor(color);
    this->rect.setOutlineColor(color);
}

GuiButton::GuiButton(sf::RenderWindow* window, GuiCommand guiCommand, const sf::Texture& texture, const sf::Texture& hovering, const sf::Texture& active, sf::Text text) :
GuiObject(window, guiCommand),
usesTexture(true),
text(text),
normalSprite(texture),
hoveringSprite(hovering),
activeSprite(active),
hovering(false),
clickedOn(false)
{}

void GuiButton::processEvent(sf::Event event)
{
    if(event.type == sf::Event::EventType::MouseButtonPressed &&
       event.mouseButton.button == sf::Mouse::Button::Left)
    {
        sf::Vector2f coords = window->mapPixelToCoords(sf::Vector2i(event.mouseButton.x,event.mouseButton.y));
        coords = getInverseTransform().transformPoint(coords);
        if(usesTexture)
        {
            if(coords.x >= 0.0f && coords.x <= normalSprite.getLocalBounds().width &&
               coords.y >= 0.0f && coords.y <= normalSprite.getLocalBounds().height)
            {
                clickedOn = true;
            }
        }
        else
        {
            if(coords.x >= 0.0f && coords.x <= rect.getSize().x &&
               coords.y >= 0.0f && coords.y <= rect.getSize().y)
            {
                clickedOn = true;
            }
        }
    }
    else if(event.type == sf::Event::EventType::MouseButtonReleased &&
            event.mouseButton.button == sf::Mouse::Button::Left &&
            clickedOn)
    {
        sf::Vector2f coords = window->mapPixelToCoords(sf::Vector2i(event.mouseButton.x,event.mouseButton.y));
        coords = getInverseTransform().transformPoint(coords);
        if(usesTexture)
        {
            if(coords.x >= 0.0f && coords.x <= normalSprite.getLocalBounds().width &&
               coords.y >= 0.0f && coords.y <= normalSprite.getLocalBounds().height)
            {
                passCommand = true;
            }
        }
        else
        {
            if(coords.x >= 0.0f && coords.x <= rect.getSize().x &&
               coords.y >= 0.0f && coords.y <= rect.getSize().y)
            {
                passCommand = true;
            }
        }

        clickedOn = false;
    }
}

GuiCommand* GuiButton::update(sf::Time time)
{
    sf::Vector2f coords = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
    coords = getInverseTransform().transformPoint(coords);

    if(usesTexture)
    {
        if(coords.x >= 0 && coords.x <= normalSprite.getLocalBounds().width &&
           coords.y >= 0 && coords.y <= normalSprite.getLocalBounds().height)
        {
            hovering = true;
        }
        else
            hovering = false;
    }
    else
    {
        if(coords.x >= 0 && coords.x <= rect.getSize().x &&
           coords.y >= 0 && coords.y <= rect.getSize().y)
        {
            hovering = true;
        }
        else
            hovering = false;

        if(clickedOn)
        {
            rect.setFillColor(activeColor);
            rect.setOutlineColor(color);
        }
        else if(hovering)
        {
            rect.setFillColor(color);
            rect.setOutlineColor(sf::Color::White);
        }
        else
        {
            rect.setFillColor(color);
            rect.setOutlineColor(color);
        }
    }

    if(passCommand)
        return guiCommand;
    else
        return NULL;
}

void GuiButton::draw(sf::RenderTarget& target, sf::RenderStates states)
{
    states.transform *= getTransform();

    if(usesTexture)
    {
        if(clickedOn)
            target.draw(activeSprite);
        else if(hovering)
            target.draw(hoveringSprite);
        else
            target.draw(normalSprite);
    }
    else
    {
        target.draw(rect);
    }

    if(text.getFont() != NULL)
        target.draw(text);
}

GuiSlider::GuiSlider(sf::RenderWindow* window, GuiCommand guiCommand, float currentValue, float llimit, float hlimit, sf::Color slider, sf::Color bg, sf::Vector2f sliderSize, sf::FloatRect sliderRect) :
GuiObject(window, guiCommand),
usesTexture(false),
rect(sliderRect),
currentValue(currentValue),
llimit(llimit),
hlimit(hlimit),
offset(),
clickedOn(false)
{
    sliderShape.setFillColor(slider);
    bgShape.setFillColor(bg);

    sliderShape.setSize(sliderSize);
    bgShape.setSize(sf::Vector2f(rect.width, rect.height / 7.0f));

    float ratio = (currentValue - llimit) / (hlimit - llimit);
    sliderShape.setPosition(ratio * (bgShape.getSize().x - sliderShape.getSize().x), rect.height / 2.0f - sliderShape.getSize().y / 2.0f);
    bgShape.setPosition(0.0f, rect.height / 2.0f - bgShape.getSize().y / 2.0f);
}

GuiSlider::GuiSlider(sf::RenderWindow* window, GuiCommand guiCommand, float currentValue, float llimit, float hlimit, const sf::Texture& slider, const sf::Texture& bg) :
GuiObject(window, guiCommand),
usesTexture(true),
sliderSprite(slider),
bgSprite(bg),
currentValue(currentValue),
llimit(llimit),
hlimit(hlimit),
offset(),
clickedOn(false)
{
    rect.left = 0.0f;
    rect.top = 0.0f;
    rect.width = bgSprite.getLocalBounds().width;
    rect.height = sliderSprite.getLocalBounds().height;

    float ratio = (currentValue - llimit) / (hlimit - llimit);
    sliderSprite.setPosition(ratio * (bgSprite.getLocalBounds().width - sliderSprite.getLocalBounds().width), rect.height / 2.0f - sliderSprite.getLocalBounds().height / 2.0f);
    bgSprite.setPosition(0.0f, rect.height / 2.0f - bgSprite.getLocalBounds().height / 2.0f);
}

void GuiSlider::processEvent(sf::Event event)
{
    if(event.type == sf::Event::EventType::MouseButtonPressed &&
       event.mouseButton.button == sf::Mouse::Button::Left)
    {
        sf::Vector2f coords = window->mapPixelToCoords(sf::Vector2i(event.mouseButton.x,event.mouseButton.y));
        coords = getInverseTransform().transformPoint(coords);
        if(usesTexture)
        {
            if(coords.x >= sliderSprite.getPosition().x &&
               coords.x <= sliderSprite.getPosition().x + sliderSprite.getLocalBounds().width &&
               coords.y >= sliderSprite.getPosition().y &&
               coords.y <= sliderSprite.getPosition().y + sliderSprite.getLocalBounds().height)
            {
                clickedOn = true;
                offset = coords.x - sliderSprite.getPosition().x;
            }
        }
        else
        {
            if(coords.x >= sliderShape.getPosition().x &&
               coords.x <= sliderShape.getPosition().x + sliderShape.getSize().x &&
               coords.y >= sliderShape.getPosition().y &&
               coords.y <= sliderShape.getPosition().y + sliderShape.getSize().y)
            {
                clickedOn = true;
                offset = coords.x - sliderShape.getPosition().x;
            }
        }
    }
    else if(event.type == sf::Event::EventType::MouseButtonReleased &&
            event.mouseButton.button == sf::Mouse::Button::Left)
    {
        clickedOn = false;
    }
}

GuiCommand* GuiSlider::update(sf::Time time)
{
    if(clickedOn)
    {
        sf::Vector2f coords = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
        coords = getInverseTransform().transformPoint(coords);

        if(usesTexture)
        {
            float pos = coords.x - offset;

            if(pos < 0.0f)
                pos = 0.0f;
            else if(pos > bgSprite.getLocalBounds().width - sliderSprite.getLocalBounds().width)
                pos = bgSprite.getLocalBounds().width - sliderSprite.getLocalBounds().width;

            sliderSprite.setPosition(pos, sliderSprite.getPosition().y);

            float ratio = pos / (bgSprite.getLocalBounds().width - sliderSprite.getLocalBounds().width);
            currentValue = ratio * (hlimit - llimit) + llimit;
        }
        else
        {
            float pos = coords.x - offset;

            if(pos < 0.0f)
                pos = 0.0f;
            else if(pos > bgShape.getSize().x - sliderShape.getSize().x)
                pos = bgShape.getSize().x - sliderShape.getSize().x;

            sliderShape.setPosition(pos, sliderShape.getPosition().y);

            float ratio = pos / (bgShape.getSize().x - sliderShape.getSize().x);
            currentValue = ratio * (hlimit - llimit) + llimit;
        }
    }

    if(passCommand || clickedOn)
    {
        guiCommand.type = GuiCommand::Type::VALUE_FLOAT;
        guiCommand.value.f = currentValue;
        return guiCommand;
    }
    else
        return NULL;
}

void GuiSlider::draw(sf::RenderTarget& target, sf::RenderStates states)
{
    states.transform *= getTransform();

    if(usesTexture)
    {
        target.draw(bgSprite, states);
        target.draw(sliderSprite, states);
    }
    else
    {
        target.draw(bgShape, states);
        target.draw(sliderShape, states);
    }
}
