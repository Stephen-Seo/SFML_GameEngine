
#ifndef NETWORK_IDENTIFIERS_HPP
#define NETWORK_IDENTIFIERS_HPP

#define GAME_PROTOCOL_ID 1357924680
#define GAME_PORT 12084
#define HEARTBEAT_MILLISECONDS 1500
#define PACKET_LOST_TIMEOUT_MILLISECONDS 1000
#define SENT_PACKET_LIST_MAX_SIZE 33
#define CONNECTION_TIMEOUT_MILLISECONDS 10000

#include <SFML/Config.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>

struct PacketInfo
{
    PacketInfo();
    PacketInfo(sf::Packet packet, sf::Uint32 ID);
    PacketInfo(sf::Packet packet, sf::Uint32 ID, sf::Uint32 address);

    sf::Packet packet;
    sf::Uint32 ID;
    union {
        sf::Clock sentTime;
        sf::Uint32 address;
    };
};

namespace network
{
    enum SpecialIDs
    {
        CONNECT = 0xFFFFFFFF,
        PING = 0xFFFFFFFE
    };

    bool moreRecent(sf::Uint32 current, sf::Uint32 previous);
    bool isSpecialID(sf::Uint32 ID);
}

#endif
