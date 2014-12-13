
#ifndef RESOURCE_HOLDER_HPP
#define RESOURCE_HOLDER_HPP

#include "resourceIdentifiers.hpp"

#include <cassert>
#include <memory>
#include <map>
#include <string>
#include <stdexcept>
#include <iostream>

#if defined(ResourcePacker_FOUND)
#include <ResourcePacker.hpp>
#endif

template <class Resource, class Identifier>
class ResourceHolder
{
public:
    ResourceHolder(GameResources::LoadingMode mode, std::string packfile = "", bool retainData = false);

    void registerResource(Identifier id, const std::string& filename);

    void load(Identifier id);
    template <class Parameter>
    void load(Identifier id, const Parameter& secondParam);

    void unload(Identifier id);

    bool isLoaded(Identifier id);

    Resource& get(Identifier id);
    const Resource& get(Identifier id) const;
private:
    friend class ResourceManager;

    std::map<Identifier, std::unique_ptr<Resource> > resourceMap;
    std::map<Identifier, std::string> pathMap;

    GameResources::LoadingMode mode;

    std::string packfile;

    bool retainData;
    std::map<Identifier, std::unique_ptr<char[]> > dataMap;
};

#include "resourceHolder.inl"
#endif
