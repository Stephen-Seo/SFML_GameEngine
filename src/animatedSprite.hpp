
#ifndef ANIMATED_SPRITE_HPP
#define ANIMATED_SPRITE_HPP

#include <vector>
#include <utility>
#include <cassert>

#include <SFML/Graphics.hpp>

class AnimatedSprite : public sf::Transformable, public sf::Drawable
{
public:
    typedef std::vector<std::pair<int, int> > SpriteState;

    enum RotationType
    {
        CYCLE,
        REVERSE
    };

    AnimatedSprite();
    AnimatedSprite(const sf::Texture& texture);

    void setTexture(const sf::Texture& texture);

    void addState(const SpriteState& state);

    void setState(int ID);
    void setState(int ID, int frame);

    void setSpriteSize(int width, int height);
    void setSpriteSize(sf::Vector2i size);

    void setTimePerFrame(float seconds);
    void setTimePerFrame(int milliseconds);

    void setRotationType(RotationType type, bool resetReversed = false);

    void updateSprite(sf::Time dt);
private:
    sf::Sprite sprite;
    bool textureLoaded;
    int currentState;
    int currentFrame;
    bool reversed;

    sf::IntRect rect;
    bool rectDirty;

    RotationType type;

    sf::Time time;
    sf::Time timePerFrame;

    std::vector<SpriteState> spriteMapping;

    bool isInitialized();
    void draw(sf::RenderTarget& target, sf::RenderStates states);
};

#endif
