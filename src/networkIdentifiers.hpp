
#ifndef NETWORK_IDENTIFIERS_HPP
#define NETWORK_IDENTIFIERS_HPP

#define GAME_PORT 12084
#define HEARTBEAT_MILLISECONDS 1500
#define PACKET_LOST_TIMEOUT_MILLISECONDS 1000

#include <SFML/Config.hpp>

namespace network
{
    bool moreRecent(sf::Uint32 current, sf::Uint32 previous);
}

#endif
