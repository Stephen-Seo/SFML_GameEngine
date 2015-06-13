
#include "networkIdentifiers.hpp"

PacketInfo::PacketInfo() :
packet(),
address(0),
ID(0),
isResending(false)
{
}

PacketInfo::PacketInfo(sf::Packet packet) :
packet(packet),
sentTime(),
address(0),
ID(0),
isResending(false)
{
}

PacketInfo::PacketInfo(sf::Packet packet, sf::Uint32 address) :
packet(packet),
address(address),
ID(0),
isResending(false)
{
}

PacketInfo::PacketInfo(sf::Packet packet, sf::Uint32 address, sf::Uint32 ID) :
packet(packet),
address(address),
ID(ID),
isResending(false)
{
}

PacketInfo::PacketInfo(sf::Packet packet, sf::Uint32 address, sf::Uint32 ID, bool isResending) :
packet(packet),
address(address),
ID(ID),
isResending(isResending)
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
