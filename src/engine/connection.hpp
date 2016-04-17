
#ifndef ENGINE_CONNECTION_HPP
#define ENGINE_CONNECTION_HPP

#define INVALID_NOTICE_TIME 5.0f

#include <unordered_map>
#include <cassert>
#include <functional>
#include <list>

#include <iostream>

#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include "networkIdentifiers.hpp"

/// Implements a UDP based connection manager as client or server.
/**
    The implementation is based on http://gafferongames.com/networking-for-game-programmers/ .
    Note that this class uses structs and such defined in "engine/networkIdentifiers.hpp".
    Thus, this class uses struct \ref ConnectionData which uses \ref PacketInfo.

    It is expected that Connection::update is called periodically from within
    a game loop with a deltaTime (time between calls to update).

    Connection maintains a queue of packets to send.
    Packets are sent periodically with an interval between 1/30th of a second
    and 1/10th of a second based on whether or not the connection is "good" or
    has a low round-trip-time.
*/
class Connection
{
public:
    /// An enum used for specifying whether or not a connection will run as "Client" or "Server".
    enum Mode
    {
        SERVER,
        CLIENT
    };

    /// Initializes based on the given mode (Client or Server) and server port.
    /**
        \param mode The enum value specifying whether or not the connection will run as Client or Server.
        \param serverPort The port of the server. If mode is "SERVER", then the UDP socket used will bind to this port. Otherwise, the "CLIENT" will connect to a server at this port and will bind to any UDP port for its socket.
    */
    Connection(Mode mode = SERVER, unsigned short serverPort = GAME_PORT);

    /// If true, then the SERVER will accept new connections and the CLIENT will accept a connection to a server.
    /**
        Set this to false to prevent connecting to any new peers.
        Note that setting this to false will prevent a Client from trying to
        connect/reconnect to a server.
    */
    bool acceptNewConnections;
    /// If true, then any packets received out of order will be ignored.
    /** Ignored packets will not call the received packet callback specified by Connection::setReceivedCallback. */
    bool ignoreOutOfSequence;
    /// If true, then timed out packets will be resent when they have timed out.
    bool resendTimedOutPackets;

    /// Checks for received packets and maintains the connection.
    /**
        It is expected for this function to be called periodically in a game's
        main loop where parameter "dt" is the deltaTime between frames.
        \param dt The deltaTime, or time between the previous call to update and now.
    */
    void update(sf::Time dt);

    /// Tells the Client the IP address of the server to connect to.
    /**
        Once the Client knows the IP address of the server, it will
        periodically attempt to establish a connection to the server unless
        Connection::acceptNewConnections is false. Connection attempts occur in
        Connection::update.
    */
    void connectToServer(sf::IpAddress address);

    /// Adds to the queue of to-send-packets the given packet to the given destination IP address.
    void sendPacket(sf::Packet& packet, sf::IpAddress address);

    /// Gets the calculated round-trip-time to an arbritrary connected peer.
    /** \return sf::Time::Zero if there are no connected peers. */
    sf::Time getRtt();
    /// Gets the calculated round-trip-time to the specified connected peer.
    /** \return sf::Time::Zero if the specified peer is not connected (not found). */
    sf::Time getRtt(sf::Uint32 address);

    /// Sets the callback called when a valid packet is received.
    /**
        The callback will be called with the packet (with connection related
        data already removed), the IP address of the sender as an uint32 (see SFML's
        sf::IpAddress) and a bool that is true when the packet was received out
        of order.
        Note that if the packet did not have any data other than what was used
        to manage the connection, then the callback will not be called.
    */
    void setReceivedCallback(std::function<void(sf::Packet, sf::Uint32, bool)> callback);

    /// Sets the callback called when a connection to a peer is established.
    /**
        The callback will be called with the IP address of the peer as an
        uint32 (see SFML's sf::IpAddress).
    */
    void setConnectedCallback(std::function<void(sf::Uint32)> callback);

    /// Sets the callback called when a connection to a peer is dropped.
    /**
        Typically, a connetion is dropped when the last received packet was
        received too long ago (timed out).
        The callback will be called with the IP address of the peer that
        has disconnected as an uint32 (see SFML's sf::IpAddress).
    */
    void setDisconnectedCallback(std::function<void(sf::Uint32)> callback);

    /// Gets a list of IP addresses of all connected peers.
    /** Note that the IP addresses is in an uint32 format (see SFML's sf::IpAddress).*/
    std::list<sf::Uint32> getConnected();

    /// Gets the size of the packet queue for the specified destination address.
    unsigned int getPacketQueueSize(sf::Uint32 destinationAddress);
    /// Clears the packet queue for the specified destination address.
    void clearPacketQueue(sf::Uint32 destinationAddress);

    /// Gets whether or not the connection is good for an arbritrary connection.
    /**
        It is expected to call this function as a Client.
        If there are no valid connections, then "false" will be returned.
        Note that if the connection is "good" then packets will be sent at an
        interval of 1/30th of a second. Otherwise packets will be sent at an
        interval of 1/10th of a second.
    */
    bool connectionIsGood();
    /// Gets whether or not the connection is good for the specified connection.
    /**
        If there is no connection to the specified IP address, then "false"
        will be returned.
        Note that if the connection is "good" then packets will be sent at an
        interval of 1/30th of a second. Otherwise packets will be sent at an
        interval of 1/10th of a second.
    */
    bool connectionIsGood(sf::Uint32 destinationAddress);

private:
    Mode mode;

    sf::UdpSocket socket;

    std::unordered_map<sf::Uint32, ConnectionData> connectionData;

    std::random_device rd;
    std::uniform_int_distribution<sf::Uint32> dist;

    sf::IpAddress clientSentAddress;
    bool clientSentAddressSet;

    std::function<void(sf::Packet, sf::Uint32, bool)> receivedCallback;
    std::function<void(sf::Uint32)> connectedCallback;
    std::function<void(sf::Uint32)> disconnectedCallback;

    bool initialized;
    bool validState;
    float invalidNoticeTimer;

    unsigned short serverPort;

    float clientRetryTimer;

    void registerConnection(sf::Uint32 address, sf::Uint32 ID, unsigned short port);
    void unregisterConnection(sf::Uint32 address);

    void shiftBitfield(sf::IpAddress address, sf::Uint32 diff);

    void checkSentPackets(sf::Uint32 ack, sf::Uint32 bitfield, sf::Uint32 address);

    void lookupRtt(sf::Uint32 address, sf::Uint32 ack);

    void checkSentPacketsSize(sf::Uint32 address);

    sf::Uint32 generateID();

    void preparePacket(sf::Packet& packet, sf::Uint32& sequenceID, sf::IpAddress address, bool isPing = false);

    void sendPacket(sf::Packet& packet, sf::IpAddress address, sf::Uint32 resendingID);

    void receivedPacket(sf::Packet packet, sf::Uint32 address, bool outOfOrder);

    void connectionMade(sf::Uint32 address);

    void connectionLost(sf::Uint32 address);

    void initialize();

};

#endif
