
template <class Resource, class Identifier>
void ResourceHolder<Resource, Identifier>::registerResource(Identifier id, const std::string& filename)
{
    if(pathMap.find(id) == pathMap.end())
        pathMap[id] = filename;
    else
        std::cout << "WARNING: Failed to register resource as ID is already registered!\n";
}

template <class Resource, class Identifier>
void ResourceHolder<Resource, Identifier>::load(Identifier id)
{
    auto pathIter = pathMap.find(id);
    if(pathIter == pathMap.end())
        throw std::runtime_error("ResourceHolder::load - ID is not registered");

    std::unique_ptr<Resource> resource(new Resource());
    if (!resource->loadFromFile(pathIter->second))
        throw std::runtime_error("ResourceHolder::load - Failed to load " + pathIter->second);
    auto inserted = resourceMap.insert(std::make_pair(id, std::move(resource)));
    assert(inserted.second);
}

template <class Resource, class Identifier>
template <class Parameter>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const Parameter& secondParam)
{
    auto pathIter = pathMap.find(id);
    if(pathIter == pathMap.end())
        throw std::runtime_error("ResourceHolder::load - ID is not registered");

    std::unique_ptr<Resource> resource(new Resource());
    if (!resource->loadFromFile(pathIter->second, secondParam))
        throw std::runtime_error("ResourceHolder::load - Failed to load " + pathIter->second);
    auto inserted = resourceMap.insert(std::make_pair(id, std::move(resource)));
    assert(inserted.second);
}

template <class Resource, class Identifier>
void ResourceHolder<Resource, Identifier>::unload(Identifier id)
{
    auto found = resourceMap.find(id);
    if(found != resourceMap.end())
    {
        resourceMap.erase(found);
    }
}

template <class Resource, class Identifier>
bool ResourceHolder<Resource, Identifier>::isLoaded(Identifier id)
{
    auto found = resourceMap.find(id);
    if(found != resourceMap.end())
        return true;
    else
        return false;
}

template <class Resource, class Identifier>
Resource& ResourceHolder<Resource, Identifier>::get(Identifier id)
{
    auto found = resourceMap.find(id);
    assert(found != resourceMap.end());
    return *found->second;
}

template <class Resource, class Identifier>
const Resource& ResourceHolder<Resource, Identifier>::get(Identifier id) const
{
    auto found = resourceMap.find(id);
    assert(found != resourceMap.end());
    return *found->second;
}
