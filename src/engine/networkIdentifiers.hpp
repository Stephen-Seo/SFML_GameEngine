
#ifndef NETWORK_IDENTIFIERS_HPP
#define NETWORK_IDENTIFIERS_HPP

#define GAME_PROTOCOL_ID 1357924680
#define GAME_PORT 12084
#define HEARTBEAT_MILLISECONDS 1500
#define PACKET_LOST_TIMEOUT_MILLISECONDS 1000
#define SENT_PACKET_LIST_MAX_SIZE 33
#define CONNECTION_TIMEOUT_MILLISECONDS 10000

#define NETWORK_GOOD_MODE_SEND_INTERVAL 1.0f/30.0f
#define NETWORK_BAD_MODE_SEND_INTERVAL 1.0f/10.0f

#include <SFML/Config.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>

struct PacketInfo
{
    PacketInfo();
    PacketInfo(sf::Packet packet);
    PacketInfo(sf::Packet packet, sf::Uint32 address);
    PacketInfo(sf::Packet packet, sf::Uint32 address, sf::Uint32 ID);

    sf::Packet packet;
    sf::Clock sentTime;
    sf::Uint32 address;
    sf::Uint32 ID;
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
