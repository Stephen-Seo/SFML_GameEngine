
#ifndef SERVER_HPP
#define SERVER_HPP

#include <list>
#include <map>
#include <cassert>

#ifndef NDEBUG
#include <iostream>
#endif

#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include "networkIdentifiers.hpp"

class Connection
{
public:
    enum Mode
    {
        SERVER,
        CLIENT
    };

    Connection();
    Connection(Mode mode);
    virtual ~Connection();

    void update(sf::Time dt);

    void connectToServer(sf::IpAddress address);

    void heartbeat();

protected:
    bool acceptNewConnections;
    bool ignoreOutOfSequence;
    bool resendTimedOutPackets;

    void preparePacket(sf::Packet& packet, sf::Uint32& sequenceID, sf::IpAddress address, bool isPing = false);

    void sendPacket(sf::Packet& packet, sf::Uint32 sequenceID, sf::IpAddress address);

    virtual void receivedPacket(sf::Packet packet, sf::Uint32 address);

    virtual void connectionMade(sf::Uint32 address);

    virtual void connectionLost(sf::Uint32 address);

private:
    Mode mode;

    sf::UdpSocket socket;

    std::map<sf::Uint32, sf::Time> heartbeatTimeMap;
    std::map<sf::Uint32, sf::Clock> elapsedTimeMap;

    std::random_device rd;
    std::uniform_int_distribution<sf::Uint32> dist;

    std::map<sf::Uint32, sf::Uint32> IDMap;

    std::map<sf::Uint32, sf::Uint32> lSequenceMap;
    std::map<sf::Uint32, sf::Uint32> rSequenceMap;

    std::map<sf::Uint32, sf::Uint32> ackBitfieldMap;

    std::map<sf::Uint32, std::list<PacketInfo> > sentPackets;

    std::list<PacketInfo> sendPacketQueue;

    std::map<sf::Uint32, sf::Time> rttMap;

    sf::IpAddress clientSentAddress;


    void registerConnection(sf::Uint32 address, sf::Uint32 ID = 0);
    void unregisterConnection(sf::Uint32 address);

    void shiftBitfield(sf::IpAddress address, sf::Uint32 diff);

    void checkSentPackets(sf::Uint32 ack, sf::Uint32 bitfield, sf::Uint32 address);

    void heartbeat(sf::Uint32 addressInteger);

    void lookupRtt(sf::Uint32 address, sf::Uint32 ack);

    void checkSentPacketsSize(sf::Uint32 address);

    sf::Uint32 generateID();


};

#endif
