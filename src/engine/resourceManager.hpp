
#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <cstdlib>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "stateStack.hpp"
#include "resourceIdentifiers.hpp"
#include "resourceHolder.hpp"

class ResourceManager
{
public:
    ResourceManager(StateStack* sstack, GameResources::LoadingMode mode, std::string packfile = "");

    sf::Texture& getTexture(const std::string& id);
    sf::Font& getFont(const std::string& id);
    sf::SoundBuffer& getSoundBuffer(const std::string& id);

    void registerTexture(State* requestor, const std::string& filename);
    void registerFont(State* requestor, const std::string& filename);
    void registerSoundBuffer(State* requestor, const std::string& filename);

    void loadResources();

    void unloadCheckResources();

private:
    TextureHolder textureHolder;
    FontHolder fontHolder;
    SoundBufferHolder soundBufferHolder;

    GameResources::LoadingMode mode;

    StateStack* sstack;

    std::unordered_map<std::size_t, std::unordered_set<std::string> > requestTextureLog;
    std::unordered_map<std::size_t, std::unordered_set<std::string> > requestFontLog;
    std::unordered_map<std::size_t, std::unordered_set<std::string> > requestSoundLog;
};

#endif
