template <class Resource>
ResourceHolder<Resource>::ResourceHolder(GameResources::LoadingMode mode, std::string packfile, bool retainData) :
mode(mode),
packfile(packfile),
retainData(retainData)
{
    assert(mode == GameResources::DEFAULT || packfile.size() > 0);
}

template <class Resource>
void ResourceHolder<Resource>::load(const std::string& id)
{
    std::unique_ptr<Resource> resource(new Resource());

    if(mode == GameResources::DEFAULT)
    {
        if (!resource->loadFromFile(id))
            throw std::runtime_error("ResourceHolder::load - Failed to load " + id);
    }
#if defined(ResourcePacker_FOUND)
    else if(mode == GameResources::PACKFILE)
    {
        std::unique_ptr<char[]> data;
        unsigned long long size;
        if(!RP::getFileData(data, size, packfile, id))
            throw std::runtime_error("ResourceHolder::load - Failed to load " + id);

        if(!resource->loadFromMemory(data.get(), size))
            throw std::runtime_error("ResourceHolder::load - Failed to load " + id);

        if(retainData)
        {
            dataMap.insert(std::make_pair(id, std::move(data)));
        }
    }
#endif

    auto inserted = resourceMap.insert(std::make_pair(id, std::move(resource)));
    assert(inserted.second);
}

template <class Resource>
template <class Parameter>
void ResourceHolder<Resource>::load(const std::string& id, const Parameter& secondParam)
{
    std::unique_ptr<Resource> resource(new Resource());

    if(mode == GameResources::DEFAULT)
    {
        if (!resource->loadFromFile(id, secondParam))
            throw std::runtime_error("ResourceHolder::load - Failed to load " + id);
    }
#if defined(ResourcePacker_FOUND)
    else if(mode == GameResources::PACKFILE)
    {
        std::unique_ptr<char[]> data;
        unsigned long long size;
        if(!RP::getFileData(data, size, packfile, id))
            throw std::runtime_error("ResourceHolder::load - Failed to load " + id);

        if(!resource->loadFromMemory(data.get(), size, secondParam))
            throw std::runtime_error("ResourceHolder::load - Failed to load " + id);

        if(retainData)
        {
            dataMap.insert(std::make_pair(id, std::move(data)));
        }
    }
#endif

    auto inserted = resourceMap.insert(std::make_pair(id, std::move(resource)));
    assert(inserted.second);
}

template <class Resource>
void ResourceHolder<Resource>::unload(const std::string& id)
{
    auto found = resourceMap.find(id);
    if(found != resourceMap.end())
    {
        if(retainData)
            dataMap.erase(found->first);
        resourceMap.erase(found);
    }
}

template <class Resource>
bool ResourceHolder<Resource>::isLoaded(const std::string& id)
{
    auto found = resourceMap.find(id);
    if(found != resourceMap.end())
        return true;
    else
        return false;
}

template <class Resource>
Resource& ResourceHolder<Resource>::get(const std::string& id)
{
    auto found = resourceMap.find(id);
    assert(found != resourceMap.end());
    return *found->second;
}

template <class Resource>
const Resource& ResourceHolder<Resource>::get(const std::string& id) const
{
    auto found = resourceMap.find(id);
    assert(found != resourceMap.end());
    return *found->second;
}
