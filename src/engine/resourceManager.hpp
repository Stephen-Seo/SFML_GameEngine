
#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <map>
#include <memory>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "stateStack.hpp"
#include "resourceIdentifiers.hpp"
#include "resourceHolder.hpp"

class ResourceManager
{
public:
    ResourceManager(StateStack* sstack, GameResources::LoadingMode mode, std::string packfile = "");

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

    GameResources::LoadingMode mode;

    std::map<std::string, std::unique_ptr<char[]> > fontDataMap;

    StateStack* sstack;
};

#endif
