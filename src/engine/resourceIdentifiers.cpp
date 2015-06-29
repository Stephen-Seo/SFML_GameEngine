
#include "resourceIdentifiers.hpp"

ResourcesSet::ResourcesSet()
{}

ResourcesSet::ResourcesSet(const ResourceSet& textureSet, const ResourceSet& fontSet, const ResourceSet& soundSet) :
textureSet(textureSet),
fontSet(fontSet),
soundSet(soundSet)
{}

