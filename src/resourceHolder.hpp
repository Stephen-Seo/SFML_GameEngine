
#ifndef RESOURCE_HOLDER_HPP
#define RESOURCE_HOLDER_HPP

#include <cassert>
#include <memory>
#include <map>
#include <string>
#include <stdexcept>
#include <iostream>

template <class Resource, class Identifier>
class ResourceHolder
{
public:
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

    std::map<Identifier, std::unique_ptr<Resource>> resourceMap;
    std::map<Identifier, std::string> pathMap;
};

#include "resourceHolder.inl"
#endif
