
#ifndef SERVER_HPP
#define SERVER_HPP

#include <unordered_map>
#include <cassert>
#include <functional>
#include <list>

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

    bool acceptNewConnections;
    bool ignoreOutOfSequence;
    bool resendTimedOutPackets;

    void update(sf::Time dt);

    void connectToServer(sf::IpAddress address);

    void heartbeat();

    void sendPacket(sf::Packet& packet, sf::IpAddress address);

    sf::Time getRtt();
    sf::Time getRtt(sf::Uint32 address);

    void setReceivedCallback(std::function<void(sf::Packet, sf::Uint32, bool)> callback);

    void setConnectedCallback(std::function<void(sf::Uint32)> callback);

    void setDisconnectedCallback(std::function<void(sf::Uint32)> callback);

    std::list<sf::Uint32> getConnected();

private:
    Mode mode;

    sf::UdpSocket socket;

    std::unordered_map<sf::Uint32, ConnectionData> connectionData;

    std::random_device rd;
    std::uniform_int_distribution<sf::Uint32> dist;

    sf::IpAddress clientSentAddress;

    std::function<void(sf::Packet, sf::Uint32, bool)> receivedCallback;
    std::function<void(sf::Uint32)> connectedCallback;
    std::function<void(sf::Uint32)> disconnectedCallback;

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

    void receivedPacket(sf::Packet packet, sf::Uint32 address, bool outOfOrder);

    void connectionMade(sf::Uint32 address);

    void connectionLost(sf::Uint32 address);

};

#endif
