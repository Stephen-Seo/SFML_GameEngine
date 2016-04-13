
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

ConnectionData::ConnectionData() :
rSequence(0),
ackBitfield(0xFFFFFFFF),
triggerSend(false),
timer(0.0f),
isGood(false),
isGoodRtt(false),
toggleTime(30.0f),
toggleTimer(0.0f),
toggledTimer(0.0f)
{}

ConnectionData::ConnectionData(sf::Uint32 ID, sf::Uint32 lSequence, unsigned short port) :
ID(ID),
lSequence(lSequence),
rSequence(0),
ackBitfield(0xFFFFFFFF),
triggerSend(false),
timer(0.0f),
isGood(false),
isGoodRtt(false),
toggleTime(30.0f),
toggleTimer(0.0f),
toggledTimer(0.0f),
port(port)
{}

bool ConnectionData::operator== (const ConnectionData& other) const
{
    return ID == other.ID;
}

std::size_t std::hash<ConnectionData>::operator() (const ConnectionData& connectionData) const
{
    return connectionData.ID;
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
