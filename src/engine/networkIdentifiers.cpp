
#include "networkIdentifiers.hpp"

PacketInfo::PacketInfo() :
packet(),
ID(0),
address(0)
{
}

PacketInfo::PacketInfo(sf::Packet packet, sf::Uint32 ID) :
packet(packet),
ID(ID),
sentTime()
{
}

PacketInfo::PacketInfo(sf::Packet packet, sf::Uint32 ID, sf::Uint32 address) :
packet(packet),
ID(ID),
address(address)
{
}

bool network::moreRecent(sf::Uint32 current, sf::Uint32 previous)
{
    return (((current > previous) && (current - previous <= 0x7FFFFFFF)) ||
            ((previous > current) && (previous - current > 0x7FFFFFFF)));
}

bool network::isSpecialID(sf::Uint32 ID)
{
    return ID == CONNECT || ID == PING;
}
