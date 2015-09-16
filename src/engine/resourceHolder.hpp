
#ifndef RESOURCE_HOLDER_HPP
#define RESOURCE_HOLDER_HPP

#include "resourceIdentifiers.hpp"

#include <cassert>
#include <memory>
#include <unordered_map>
#include <string>
#include <stdexcept>
#include <iostream>

#if defined(ResourcePacker_FOUND)
#include <ResourcePacker.hpp>
#endif

template <class Resource>
class ResourceHolder
{
public:
    ResourceHolder(GameResources::LoadingMode mode, std::string packfile = "", bool retainData = false);

    void load(const std::string& id);
    template <class Parameter>
    void load(const std::string& id, const Parameter& secondParam);

    void unload(const std::string& id);

    bool isLoaded(const std::string& id);

    Resource& get(const std::string& id);
    const Resource& get(const std::string& id) const;
private:
    friend class ResourceManager;

    std::unordered_map<std::string, std::unique_ptr<Resource> > resourceMap;

    GameResources::LoadingMode mode;

    std::string packfile;

    bool retainData;
    std::unordered_map<std::string, std::unique_ptr<char[]> > dataMap;
};

#include <engine/resourceHolder.inl>
#endif
