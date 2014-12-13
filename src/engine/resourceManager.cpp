
#include "resourceManager.hpp"

#ifndef N_DEBUG
    #include <iostream>
#endif

ResourceManager::ResourceManager(StateStack* sstack, GameResources::LoadingMode mode, std::string packfile) :
textureHolder(mode, packfile, false),
fontHolder(mode, packfile, true),
soundBufferHolder(mode, packfile, false),
mode(mode),
sstack(sstack)
{}

sf::Texture& ResourceManager::getTexture(Textures::ID id)
{
    return textureHolder.get(id);
}

sf::Font& ResourceManager::getFont(Fonts::ID id)
{
    return fontHolder.get(id);
}

sf::SoundBuffer& ResourceManager::getSoundBuffer(Sound::ID id)
{
    return soundBufferHolder.get(id);
}

void ResourceManager::registerTexture(Textures::ID id, const std::string& filename)
{
    textureHolder.registerResource(id, filename);
}

void ResourceManager::registerFont(Fonts::ID id, const std::string& filename)
{
    fontHolder.registerResource(id, filename);
}

void ResourceManager::registerSoundBuffer(Sound::ID id, const std::string& filename)
{
    soundBufferHolder.registerResource(id, filename);
}

void ResourceManager::loadResources(ResourcesSet rset)
{
    // load textures if not loaded
    for(auto iter = rset.tset.begin(); iter != rset.tset.end(); ++iter)
    {
        if(!textureHolder.isLoaded(*iter))
        {
            textureHolder.load(*iter);
        }
    }

    // load fonts if not loaded
    for(auto iter = rset.fset.begin(); iter != rset.fset.end(); ++iter)
    {
        if(!fontHolder.isLoaded(*iter))
        {
            fontHolder.load(*iter);
        }
    }

    // load sound buffers if not loaded
    for(auto iter = rset.sset.begin(); iter != rset.sset.end(); ++iter)
    {
        if(!soundBufferHolder.isLoaded(*iter))
        {
            soundBufferHolder.load(*iter);
        }
    }

#ifndef N_DEBUG
    std::cout << "On Load:\n";
    std::cout << "\tTextures Loaded: " << textureHolder.resourceMap.size() << "\n";
    std::cout << "\tFonts Loaded: " << fontHolder.resourceMap.size() << "\n";
    std::cout << "\tSoundBuffers Loaded: " << soundBufferHolder.resourceMap.size() << "\n" << std::flush;
#endif
}

void ResourceManager::unloadCheckResources()
{
    ResourcesSet resourcesSet = sstack->getNeededResources();

    // check for unneeded textures
    for(auto iter = textureHolder.resourceMap.begin(); iter != textureHolder.resourceMap.end();)
    {
        if(resourcesSet.tset.find(iter->first) == resourcesSet.tset.end())
        {
            iter = textureHolder.resourceMap.erase(iter);
        }
        else
        {
            ++iter;
        }
    }

    // check for unneeded fonts
    for(auto iter = fontHolder.resourceMap.begin(); iter != fontHolder.resourceMap.end();)
    {
        if(resourcesSet.fset.find(iter->first) == resourcesSet.fset.end())
        {
            iter = fontHolder.resourceMap.erase(iter);
        }
        else
        {
            ++iter;
        }
    }

    // check for unneeded sound buffers
    for(auto iter = soundBufferHolder.resourceMap.begin(); iter != soundBufferHolder.resourceMap.end();)
    {
        if(resourcesSet.sset.find(iter->first) == resourcesSet.sset.end())
        {
            iter = soundBufferHolder.resourceMap.erase(iter);
        }
        else
        {
            ++iter;
        }
    }

#ifndef N_DEBUG
    std::cout << "On Unload:\n";
    std::cout << "\tTextures Loaded: " << textureHolder.resourceMap.size() << "\n";
    std::cout << "\tFonts Loaded: " << fontHolder.resourceMap.size() << "\n";
    std::cout << "\tSoundBuffers Loaded: " << soundBufferHolder.resourceMap.size() << "\n" << std::flush;
#endif
}

