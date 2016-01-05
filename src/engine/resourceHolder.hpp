
#ifndef RESOURCE_HOLDER_HPP
#define RESOURCE_HOLDER_HPP

#include "resourceIdentifiers.hpp"

#include <cassert>
#include <memory>
#include <unordered_map>
#include <string>
#include <stdexcept>
#include <iostream>

#include <ResourcePacker.hpp>

template <class Resource>
class ResourceHolder
{
public:
    ResourceHolder(GameResources::LoadingMode mode, std::string packfile = "", bool retainData = true);

    void load(const std::string& id);
    void loadFromPackfile(const std::string& id, const std::string& packfile);
    template <class Parameter>
    void load(const std::string& id, const Parameter& secondParam);
    template <class Parameter>
    void loadFromPackfile(const std::string& id, const std::string& packfile, const Parameter& secondParam);

    void unload(const std::string& id);

    bool isLoaded(const std::string& id);

    Resource& get(const std::string& id);
    const Resource& get(const std::string& id) const;
private:
    friend class ResourceManager;

    std::unordered_map<std::string, std::unique_ptr<Resource> > resourceMap;

    GameResources::LoadingMode mode;

    bool retainData;
    std::unordered_map<std::string, std::unique_ptr<char[]> > dataMap;
};

#include <engine/resourceHolder.inl>
#endif
