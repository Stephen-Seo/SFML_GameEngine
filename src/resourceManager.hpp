
#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "stateStack.hpp"
#include "resourceIdentifiers.hpp"

class ResourceManager
{
public:
    ResourceManager(StateStack* sstack);

    sf::Texture& getTexture(Textures::ID id);
    sf::Font& getFont(Fonts::ID id);
    sf::SoundBuffer& getSoundBuffer(Sound::ID id);

    void registerTexture(Textures::ID id, const std::string& filename);
    void registerFont(Fonts::ID id, const std::string& filename);
    void registerSoundBuffer(Sound::ID id, const std::string& filename);

    void loadResources(ResourcesSet rset);

    void unloadCheckResources();

private:
    TextureHolder textureHolder;
    FontHolder fontHolder;
    SoundBufferHolder soundBufferHolder;

    StateStack* sstack;
};

#endif
