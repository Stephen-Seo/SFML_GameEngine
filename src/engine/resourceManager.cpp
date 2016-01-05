
#include "resourceManager.hpp"

#include <typeinfo>
#include <vector>
#include <list>

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

sf::Texture& ResourceManager::getTexture(const std::string& id)
{
    return textureHolder.get(id);
}

sf::Font& ResourceManager::getFont(const std::string& id)
{
    return fontHolder.get(id);
}

sf::SoundBuffer& ResourceManager::getSoundBuffer(const std::string& id)
{
    return soundBufferHolder.get(id);
}

void ResourceManager::registerTexture(State& requestor, const std::string& filename, const std::string& packfile)
{
#ifndef NDEBUG
    std::cout << typeid(requestor).name() << ':' << typeid(requestor).hash_code() << " requested " << filename << '\n';
#endif
    requestTextureLog[typeid(requestor).hash_code()].insert(std::make_pair(filename, packfile));
}

void ResourceManager::registerFont(State& requestor, const std::string& filename, const std::string& packfile)
{
#ifndef NDEBUG
    std::cout << typeid(requestor).name() << ':' << typeid(requestor).hash_code() << " requested " << filename << '\n';
#endif
    requestFontLog[typeid(requestor).hash_code()].insert(std::make_pair(filename, packfile));
}

void ResourceManager::registerSoundBuffer(State& requestor, const std::string& filename, const std::string& packfile)
{
#ifndef NDEBUG
    std::cout << typeid(requestor).name() << ':' << typeid(requestor).hash_code() << " requested " << filename << '\n';
#endif
    requestSoundLog[typeid(requestor).hash_code()].insert(std::make_pair(filename, packfile));
}

void ResourceManager::loadResources()
{
    std::vector<std::size_t> hashCodes = sstack->getContentsHashCodes();

#ifndef NDEBUG
    std::cout << "Loaded states: ";
#endif
    for(auto hashIter = hashCodes.begin(); hashIter != hashCodes.end(); ++hashIter)
    {
#ifndef NDEBUG
        std::cout << *hashIter << ' ';
#endif
        for(auto textureLogIter = requestTextureLog[*hashIter].begin(); textureLogIter != requestTextureLog[*hashIter].end(); ++textureLogIter)
        {
            if(!textureHolder.isLoaded(textureLogIter->first))
            {
                if(textureLogIter->second.empty())
                {
                    textureHolder.load(textureLogIter->first);
                }
                else
                {
                    textureHolder.loadFromPackfile(textureLogIter->first, textureLogIter->second);
                }
            }
        }

        for(auto fontLogIter = requestFontLog[*hashIter].begin(); fontLogIter != requestFontLog[*hashIter].end(); ++fontLogIter)
        {
            if(!fontHolder.isLoaded(fontLogIter->first))
            {
                if(fontLogIter->second.empty())
                {
                    fontHolder.load(fontLogIter->first);
                }
                else
                {
                    fontHolder.loadFromPackfile(fontLogIter->first, fontLogIter->second);
                }
            }
        }

        for(auto soundLogIter = requestSoundLog[*hashIter].begin(); soundLogIter != requestSoundLog[*hashIter].end(); ++soundLogIter)
        {
            if(!soundBufferHolder.isLoaded(soundLogIter->first))
            {
                if(soundLogIter->second.empty())
                {
                    soundBufferHolder.load(soundLogIter->first);
                }
                else
                {
                    soundBufferHolder.loadFromPackfile(soundLogIter->first, soundLogIter->second);
                }
            }
        }
    }

#ifndef N_DEBUG
    std::cout << "\nOn Load:\n";
    std::cout << "\tTextures Loaded: " << textureHolder.resourceMap.size() << "\n";
    std::cout << "\tFonts Loaded: " << fontHolder.resourceMap.size() << "\n";
    std::cout << "\tSoundBuffers Loaded: " << soundBufferHolder.resourceMap.size() << "\n" << std::flush;
#endif
}

void ResourceManager::unloadCheckResources()
{
    std::vector<std::size_t> hashCodes = sstack->getContentsHashCodes();
    std::list<std::unordered_map<std::string, std::string>*> requiredTextures;
    std::list<std::unordered_map<std::string, std::string>*> requiredFonts;
    std::list<std::unordered_map<std::string, std::string>*> requiredSounds;

    for(auto hashIter = hashCodes.begin(); hashIter != hashCodes.end(); ++hashIter)
    {
        requiredTextures.push_back(&(requestTextureLog[*hashIter]));
        requiredFonts.push_back(&(requestFontLog[*hashIter]));
        requiredSounds.push_back(&(requestSoundLog[*hashIter]));
    }

    bool isRequired;

    for(auto textureIter = textureHolder.resourceMap.begin(); textureIter != textureHolder.resourceMap.end();)
    {
        isRequired = false;
        for(auto requiredIter = requiredTextures.begin(); requiredIter != requiredTextures.end(); ++requiredIter)
        {
            if((*requiredIter)->find(textureIter->first) != (*requiredIter)->end())
            {
                isRequired = true;
                break;
            }
        }

        if(isRequired)
        {
            ++textureIter;
        }
        else
        {
            textureIter = textureHolder.resourceMap.erase(textureIter);
        }
    }

    for(auto fontIter = fontHolder.resourceMap.begin(); fontIter != fontHolder.resourceMap.end();)
    {
        isRequired = false;
        for(auto requiredIter = requiredFonts.begin(); requiredIter != requiredFonts.end(); ++requiredIter)
        {
            if((*requiredIter)->find(fontIter->first) != (*requiredIter)->end())
            {
                isRequired = true;
                break;
            }
        }

        if(isRequired)
        {
            ++fontIter;
        }
        else
        {
            fontIter = fontHolder.resourceMap.erase(fontIter);
        }
    }

    for(auto soundIter = soundBufferHolder.resourceMap.begin(); soundIter != soundBufferHolder.resourceMap.end();)
    {
        isRequired = false;
        for(auto requiredIter = requiredSounds.begin(); requiredIter != requiredSounds.end(); ++requiredIter)
        {
            if((*requiredIter)->find(soundIter->first) != (*requiredIter)->end())
            {
                isRequired = true;
                break;
            }
        }

        if(isRequired)
        {
            ++soundIter;
        }
        else
        {
            soundIter = soundBufferHolder.resourceMap.erase(soundIter);
        }
    }

#ifndef N_DEBUG
    std::cout << "On Unload:\n";
    std::cout << "\tTextures Loaded: " << textureHolder.resourceMap.size() << "\n";
    std::cout << "\tFonts Loaded: " << fontHolder.resourceMap.size() << "\n";
    std::cout << "\tSoundBuffers Loaded: " << soundBufferHolder.resourceMap.size() << "\n" << std::flush;
#endif
}

