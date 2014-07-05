
#include "networkIdentifiers.hpp"

bool network::moreRecent(sf::Uint32 current, sf::Uint32 previous)
{
    return (((current > previous) && (current - previous <= 0x7FFFFFFF)) ||
            ((previous > current) && (previous - current > 0x7FFFFFFF)));
}
