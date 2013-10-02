
#ifndef RESOURCE_IDENTIFIERS_HPP
#define RESOURCE_IDENTIFIERS_HPP

namespace sf
{
    class Texture;
    class Font;
}

namespace Textures
{
    enum ID
    {
    };
}

namespace Fonts
{
    enum ID
    {
    };
}

template<class Resource, class Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture,Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID> FontHolder;

#endif
