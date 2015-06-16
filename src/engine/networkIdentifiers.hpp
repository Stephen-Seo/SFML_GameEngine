
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

#include <list>
#include <cstdlib>

#include <SFML/Config.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>

struct PacketInfo
{
    PacketInfo();
    PacketInfo(sf::Packet packet);
    PacketInfo(sf::Packet packet, sf::Uint32 address);
    PacketInfo(sf::Packet packet, sf::Uint32 address, sf::Uint32 ID);
    PacketInfo(sf::Packet packet, sf::Uint32 address, sf::Uint32 ID, bool isResending);

    sf::Packet packet;
    sf::Clock sentTime;
    sf::Uint32 address;
    sf::Uint32 ID;
    bool isResending;
};

struct ConnectionData
{
    ConnectionData();
    ConnectionData(sf::Uint32 ID, sf::Uint32 lSequence);

    sf::Clock elapsedTime;
    sf::Uint32 ID;
    sf::Uint32 lSequence;
    sf::Uint32 rSequence;
    sf::Uint32 ackBitfield;
    std::list<PacketInfo> sentPackets;
    std::list<PacketInfo> sendPacketQueue;
    sf::Time rtt;
    bool triggerSend;
    float timer;
    bool isGood;
    bool isGoodRtt;
    float toggleTime;
    float toggleTimer;
    float toggledTimer;

    bool operator== (const ConnectionData& other) const;
};

namespace std
{
    template<>
    struct hash<ConnectionData>
    {
        std::size_t operator() (const ConnectionData& connectionData) const;
    };
}

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
