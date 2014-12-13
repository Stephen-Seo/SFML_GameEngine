
#include "animatedSprite.hpp"

AnimatedSprite::AnimatedSprite() :
sprite(),
textureLoaded(false),
currentState(0),
currentFrame(0),
reversed(false),
rectDirty(true),
type(CYCLE)
{
}

AnimatedSprite::AnimatedSprite(const sf::Texture& texture) :
sprite(texture),
textureLoaded(true),
currentState(0),
currentFrame(0),
reversed(false),
rectDirty(true),
type(CYCLE)
{
}

void AnimatedSprite::setTexture(const sf::Texture& texture)
{
    sprite.setTexture(texture, true);
    textureLoaded = true;
    if(currentState < spriteMapping.size() && currentFrame < spriteMapping[currentState].size())
    {
        rect.left = spriteMapping[currentState][currentFrame].first;
        rect.top = spriteMapping[currentState][currentFrame].second;
        rectDirty = true;
    }
}

void AnimatedSprite::addState(const SpriteState& state)
{
    spriteMapping.push_back(state);
}

void AnimatedSprite::setState(int ID)
{
    setState(ID, 0);
}

void AnimatedSprite::setState(int ID, int frame)
{
    if(ID >= 0 && ID < spriteMapping.size() && frame >= 0 && frame < spriteMapping[ID].size())
    {
        currentState = ID;
        currentFrame = frame;
        rect.left = spriteMapping[currentState][currentFrame].first;
        rect.top = spriteMapping[currentState][currentFrame].second;
        rectDirty = true;
    }
}

void AnimatedSprite::setSpriteSize(int width, int height)
{
    rectDirty = true;
    rect.width = width;
    rect.height = height;
}

void AnimatedSprite::setSpriteSize(sf::Vector2i size)
{
    setSpriteSize(size.x, size.y);
}

void AnimatedSprite::setTimePerFrame(float seconds)
{
    timePerFrame = sf::seconds(seconds);
}

void AnimatedSprite::setTimePerFrame(int milliseconds)
{
    timePerFrame = sf::milliseconds(milliseconds);
}

void AnimatedSprite::setRotationType(RotationType type, bool resetReversed)
{
    this->type = type;
    if(resetReversed)
    {
        reversed = false;
    }
}

void AnimatedSprite::updateSprite(sf::Time dt)
{
    time += dt;
    while(time > timePerFrame)
    {
        switch(type)
        {
        case CYCLE:
            currentFrame = (currentFrame + 1) % spriteMapping[currentState].size();
            break;
        case REVERSE:
            currentFrame = currentFrame + (reversed ? -1 : 1);
            if(currentFrame < 0 || currentFrame >= spriteMapping[currentState].size())
            {
                currentFrame = currentFrame + (reversed ? 2 : -2);
                reversed = !reversed;
            }
            break;
        default:
            break;
        }

        rect.left = spriteMapping[currentState][currentFrame].first;
        rect.top = spriteMapping[currentState][currentFrame].second;
        rectDirty = true;
        time -= timePerFrame;
    }
}

bool AnimatedSprite::isInitialized()
{
    return textureLoaded && spriteMapping.size() > 0;
}

void AnimatedSprite::draw(sf::RenderTarget& target, sf::RenderStates states)
{
    assert(isInitialized());

    states.transform *= getTransform();

    if(rectDirty)
    {
        sprite.setTextureRect(rect);
        rectDirty = false;
    }

    target.draw(sprite, states);
}
