
#ifndef RESOURCE_IDENTIFIERS_HPP
#define RESOURCE_IDENTIFIERS_HPP

#include <cstdlib>
#include <unordered_set>

namespace sf
{
    class Texture;
    class Font;
    class SoundBuffer;
}

template<class Resource>
class ResourceHolder;

typedef ResourceHolder<sf::Texture> TextureHolder;
typedef ResourceHolder<sf::Font> FontHolder;
typedef ResourceHolder<sf::SoundBuffer> SoundBufferHolder;

typedef std::unordered_set<std::string> ResourceSet;

struct ResourcesSet
{
    ResourcesSet();

    ResourcesSet(const ResourceSet& textureSet, const ResourceSet& fontSet, const ResourceSet& soundSet);

    ResourceSet textureSet;
    ResourceSet fontSet;
    ResourceSet soundSet;
};

#endif

