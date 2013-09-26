
#ifndef RESOURCE_HOLDER_HPP
#define RESOURCE_HOLDER_HPP

#include <memory>
#include <map>
#include <string>
#include <stdexcept>

template <class Resource, class Identifier>
class ResourceHolder
{
public:
    void load(Identifier id, const std::string& filename);
    template <class Parameter>
    void load(Identifier id, const std::string& filename, const Parameter& secondParam);

    Resource& get(Identifier id);
    const Resource& get(Identifier id) const;
private:
    std::map<Identifier, std::unique_ptr<Resource>> resourceMap;
};

#include "resourceHolder.inl"
#endif
