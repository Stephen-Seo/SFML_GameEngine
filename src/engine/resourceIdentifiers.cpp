
#include "resourceIdentifiers.hpp"

ResourcesSet::ResourcesSet() :
tset(),
fset(),
sset()
{}

ResourcesSet::ResourcesSet(const TextureSet& tset, const FontSet& fset, const SoundSet& sset) :
tset(tset),
fset(fset),
sset(sset)
{}
