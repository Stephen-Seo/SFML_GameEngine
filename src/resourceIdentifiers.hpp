
#ifndef RESOURCE_IDENTIFIERS_HPP
#define RESOURCE_IDENTIFIERS_HPP

#include <cstdlib>
#include <set>

namespace sf
{
    class Texture;
    class Font;
    class SoundBuffer;
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

namespace Music
{
    enum ID
    {
    };
}

namespace Sound
{
    enum ID
    {
    };
}

namespace GameResources
{
    enum LoadingMode
    {
        DEFAULT,
        PACKFILE
    };
}

template<class Resource, class Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID> FontHolder;
typedef ResourceHolder<sf::SoundBuffer, Sound::ID> SoundBufferHolder;

typedef std::set<Textures::ID> TextureSet;
typedef std::set<Fonts::ID> FontSet;
typedef std::set<Sound::ID> SoundSet;

struct ResourcesSet
{
    ResourcesSet();

    ResourcesSet(TextureSet& tset, FontSet& fset, SoundSet& sset);

    TextureSet* tset;
    FontSet* fset;
    SoundSet* sset;
};
#endif
