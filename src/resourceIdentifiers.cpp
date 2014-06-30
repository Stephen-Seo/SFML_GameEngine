
#include "resourceIdentifiers.hpp"

ResourcesSet::ResourcesSet() :
tset(NULL),
fset(NULL),
sset(NULL)
{}

ResourcesSet::ResourcesSet(TextureSet& tset, FontSet& fset, SoundSet& sset) :
tset(&tset),
fset(&fset),
sset(&sset)
{}
