
#ifndef SERVER_HPP
#define SERVER_HPP

#include <unordered_map>
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

    void sendPacket(sf::Packet& packet, sf::IpAddress address);

    sf::Time getRtt();
    sf::Time getRtt(sf::Uint32 address);

    virtual void receivedPacket(sf::Packet packet, sf::Uint32 address);

    virtual void connectionMade(sf::Uint32 address);

    virtual void connectionLost(sf::Uint32 address);

private:
    Mode mode;

    sf::UdpSocket socket;

    std::unordered_map<sf::Uint32, ConnectionData> connectionData;

    std::random_device rd;
    std::uniform_int_distribution<sf::Uint32> dist;

    sf::IpAddress clientSentAddress;

    void registerConnection(sf::Uint32 address, sf::Uint32 ID = 0);
    void unregisterConnection(sf::Uint32 address);

    void shiftBitfield(sf::IpAddress address, sf::Uint32 diff);

    void checkSentPackets(sf::Uint32 ack, sf::Uint32 bitfield, sf::Uint32 address);

    void heartbeat(sf::Uint32 addressInteger);

    void lookupRtt(sf::Uint32 address, sf::Uint32 ack);

    void checkSentPacketsSize(sf::Uint32 address);

    sf::Uint32 generateID();

    void preparePacket(sf::Packet& packet, sf::Uint32& sequenceID, sf::IpAddress address, bool isPing = false);

    void sendPacket(sf::Packet& packet, sf::IpAddress address, sf::Uint32 resendingID);

};

#endif
