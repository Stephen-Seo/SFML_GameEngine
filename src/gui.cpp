
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

GuiCommand::Ptr::Ptr(bool* b) :
b(b)
{}

GuiCommand::Ptr::Ptr(int* i) :
i(i)
{}

GuiCommand::Ptr::Ptr(float* f) :
f(f)
{}

GuiCommand::GuiCommand(GuiCommand::Type type, GuiCommand::Value value, GuiCommand::Ptr ptr) :
type(type),
value(value),
ptr(ptr)
{}

GuiObject::GuiObject(sf::RenderWindow* window, GuiCommand guiCommand) :
passCommand(false),
guiCommand(guiCommand),
window(window)
{}

GuiObject::~GuiObject()
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
    {
        passCommand = false;
        return &guiCommand;
    }
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
            event.mouseButton.button == sf::Mouse::Button::Left &&
            clickedOn)
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

    return returnValue();
}

GuiCommand* GuiSlider::returnValue()
{
    if(passCommand || clickedOn)
    {
        guiCommand.type = GuiCommand::Type::VALUE_FLOAT;
        guiCommand.value.f = currentValue;
        passCommand = false;
        return &guiCommand;
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

GuiIntSlider::GuiIntSlider(sf::RenderWindow* window, GuiCommand guiCommand, float currentValue, float llimit, float hlimit, sf::Color slider, sf::Color bg, sf::Vector2f sliderSize, sf::FloatRect sliderRect) :
GuiSlider(window, guiCommand, currentValue, llimit, hlimit, slider, bg, sliderSize, sliderRect)
{}

GuiIntSlider::GuiIntSlider(sf::RenderWindow* window, GuiCommand guiCommand, float currentValue, float llimit, float hlimit, const sf::Texture& slider, const sf::Texture& bg) :
GuiSlider(window, guiCommand, currentValue, llimit, hlimit, slider, bg)
{}

GuiCommand* GuiIntSlider::returnValue()
{
    if(passCommand || clickedOn)
    {
        guiCommand.type = GuiCommand::Type::VALUE_INT;
        guiCommand.value.i = int(currentValue + 0.5f);
        passCommand = false;
        return &guiCommand;
    }
    else
        return NULL;
}

GuiCheckbox::GuiCheckbox(sf::RenderWindow* window, GuiCommand guiCommand, bool initialValue, sf::Color color, sf::Vector2f size) :
GuiObject(window, guiCommand),
usesTexture(false),
currentValue(initialValue),
clickedOn(false),
checkLines(sf::PrimitiveType::Lines, 4)
{
    boxShape.setFillColor(sf::Color::Transparent);
    boxShape.setOutlineColor(color);
    boxShape.setOutlineThickness(3.0f);
    boxShape.setSize(size);

    checkLines[0].position = sf::Vector2f(0.0f, 0.0f);
    checkLines[1].position = sf::Vector2f(size.x, size.y);
    checkLines[2].position = sf::Vector2f(size.x, 0.0f);
    checkLines[3].position = sf::Vector2f(0.0f, size.y);

    for(int i = 0; i < 4; ++i)
        checkLines[i].color = color;
}

GuiCheckbox::GuiCheckbox(sf::RenderWindow* window, GuiCommand guiCommand, bool initialValue, const sf::Texture& box, const sf::Texture& check) :
GuiObject(window, guiCommand),
usesTexture(true),
currentValue(initialValue),
clickedOn(false),
boxSprite(box),
checkSprite(check)
{}

void GuiCheckbox::processEvent(sf::Event event)
{
    if(event.type == sf::Event::EventType::MouseButtonPressed &&
       event.mouseButton.button == sf::Mouse::Button::Left)
    {
        sf::Vector2f coords = window->mapPixelToCoords(sf::Vector2i(event.mouseButton.x,event.mouseButton.y));
        coords = getInverseTransform().transformPoint(coords);

        if(usesTexture)
        {
            if(coords.x >= 0.0f && coords.x <= boxSprite.getLocalBounds().width &&
               coords.y >= 0.0f && coords.y <= boxSprite.getLocalBounds().height)
            {
                clickedOn = true;
            }
        }
        else
        {
            if(coords.x >= 0.0f && coords.x <= boxShape.getSize().x &&
               coords.y >= 0.0f && coords.y <= boxShape.getSize().y)
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
            if(coords.x >= 0.0f && coords.x <= boxSprite.getLocalBounds().width &&
               coords.y >= 0.0f && coords.y <= boxSprite.getLocalBounds().height)
            {
                currentValue = !currentValue;
                passCommand = true;
            }
        }
        else
        {
            if(coords.x >= 0.0f && coords.x <= boxShape.getSize().x &&
               coords.y >= 0.0f && coords.y <= boxShape.getSize().y)
            {
                currentValue = !currentValue;
                passCommand = true;
            }
        }

        clickedOn = false;
    }
}

GuiCommand* GuiCheckbox::update(sf::Time time)
{
    if(passCommand)
    {
        passCommand = false;
        guiCommand.type = GuiCommand::Type::VALUE_BOOL;
        guiCommand.value.b = currentValue;
        return &guiCommand;
    }
    else
        return NULL;
}

void GuiCheckbox::draw(sf::RenderTarget& target, sf::RenderStates states)
{
    states.transform *= getTransform();

    if(usesTexture)
    {
        target.draw(boxSprite, states);
        if(currentValue)
        {
            target.draw(checkSprite, states);
        }
    }
    else
    {
        target.draw(boxShape, states);
        if(currentValue)
        {
            target.draw(checkLines, states);
        }
    }
}


GuiText::GuiText(sf::RenderWindow* window, GuiCommand guiCommand, sf::Text text) :
GuiObject(window, guiCommand),
text(text)
{}

void GuiText::processEvent(sf::Event event)
{}

GuiCommand* GuiText::update(sf::Time time)
{
    return NULL;
}

void GuiText::draw(sf::RenderTarget& target, sf::RenderStates states)
{
    states.transform *= getTransform();

    target.draw(text, states);
}

GuiImage::GuiImage(sf::RenderWindow* window, GuiCommand guiCommand, const sf::Texture& texture) :
GuiObject(window, guiCommand),
sprite(texture)
{}

void GuiImage::processEvent(sf::Event event)
{}

GuiCommand* GuiImage::update(sf::Time time)
{
    return NULL;
}

void GuiImage::draw(sf::RenderTarget& target, sf::RenderStates states)
{
    states.transform *= getTransform();

    target.draw(sprite, states);
}

GuiSystem::GuiSystem()
{}

void GuiSystem::processEvent(sf::Event event)
{
    for(auto iter = guiList.begin(); iter != guiList.end(); ++iter)
    {
        (*iter)->processEvent(event);
    }
}

void GuiSystem::update(sf::Time time, std::function<void(States::ID)> requestStackPush, std::function<void()> requestStackPop)
{
    for(auto iter = guiList.begin(); iter != guiList.end(); ++iter)
    {
        GuiCommand* command = (*iter)->update(time);
        if(command != NULL)
        {
            if(command->type == GuiCommand::Type::STATE)
            {
                if(command->value.id == States::ID::None)
                {
                    requestStackPop();
                }
                else
                {
                    requestStackPush(command->value.id);
                }
            }
            else if(command->type == GuiCommand::Type::VALUE_BOOL)
            {
                assert(command->ptr.b);
                *(command->ptr.b) = command->value.b;
            }
            else if(command->type == GuiCommand::Type::VALUE_INT)
            {
                assert(command->ptr.i);
                *(command->ptr.i) = command->value.i;
            }
            else if(command->type == GuiCommand::Type::VALUE_FLOAT)
            {
                assert(command->ptr.f);
                *(command->ptr.f) = command->value.f;
            }
        }
    }
}

void GuiSystem::draw(sf::RenderWindow& window)
{
    for(auto iter = guiList.begin(); iter != guiList.end(); ++iter)
    {
        window.draw(*(*iter));
    }
}

void GuiSystem::add(GuiObject* guiObject)
{
    GuiObject::Ptr guiObjectPtr(guiObject);
    add(guiObjectPtr);
}

void GuiSystem::add(GuiObject::Ptr& guiObject)
{
    guiList.push_back(std::move(guiObject));
}

void GuiSystem::clear()
{
    guiList.clear();
}
