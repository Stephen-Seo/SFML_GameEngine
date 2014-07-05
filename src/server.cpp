
#include "server.hpp"

Server::Server()
{
    socket.bind(GAME_PORT);
    socket.setBlocking(false);
}

Server::~Server()
{
}

void Server::update(sf::Time dt)
{
    for(auto tIter = elapsedTimeMap.begin(); tIter != elapsedTimeMap.end(); ++tIter)
    {
        tIter->second += dt;
        if(tIter->second.asMilliseconds() > HEARTBEAT_MILLISECONDS)
        {
            tIter->second -= sf::milliseconds(HEARTBEAT_MILLISECONDS);
            heartbeat(tIter->first);
            // TODO ???
        }
    }

    sf::Packet packet;
    sf::IpAddress address;
    unsigned short remotePort;
    sf::Socket::Status status = socket.receive(packet, address, remotePort);
    if(status == sf::Socket::Done)
    {
        sf::Uint32 ID;
        sf::Uint32 sequence;
        sf::Uint32 ack;
        sf::Uint32 ackBitfield;

        packet >> ID;
        packet >> sequence;
        packet >> ack;
        packet >> ackBitfield;

        // Establish connection
        if(ID == 0xFFFFFFFF)
        {
            registerClient(address);
            sf::Packet newPacket = preparePacket(address);
            sendPacket(newPacket, address);
            return;
        }

        //TODO generalized packet handling
    }
}

void Server::receivedPacket(sf::Packet packet)
{}

sf::Packet Server::preparePacket(sf::IpAddress address)
{
    sf::Packet packet;

    auto iter = IDMap.find(address.toInteger());
    if(iter == IDMap.end())
        return packet;

    sf::Uint32 ID = iter->second;

    sf::Uint32 lSequence = lSequenceMap[address.toInteger()]++;

    sf::Uint32 ack = rAckMap[address.toInteger()];

    sf::Uint32 ackBitfield = ackBitfieldMap[address.toInteger()];

    packet << ID << lSequence << ack << ackBitfield;

    return packet;
}

void Server::sendPacket(sf::Packet& packet, sf::IpAddress address)
{
    socket.send(packet, address, GAME_PORT);
}

void Server::registerClient(sf::IpAddress address)
{
    elapsedTimeMap.insert(std::make_pair(address.toInteger(), sf::Time()));

    IDMap.insert(std::make_pair(address.toInteger(), generateID()));

    lSequenceMap.insert(std::make_pair(address.toInteger(), (sf::Uint32) 0));
    rSequenceMap.insert(std::make_pair(address.toInteger(), (sf::Uint32) 0));

    rAckMap.insert(std::make_pair(address.toInteger(), (sf::Uint32) 0));
    ackBitfieldMap.insert(std::make_pair(address.toInteger(), (sf::Uint32) 0xFFFFFFFF));

    missedAckQueueMap.insert(std::make_pair(address.toInteger(), std::list<std::pair<sf::Uint32, sf::Clock> >()));
}

void Server::heartbeat(sf::Uint32 addressInteger)
{
    sf::IpAddress address(addressInteger);

    //TODO
}

sf::Uint32 Server::generateID()
{
    sf::Uint32 id;
    do
    {
        id = dist(rd);
    } while (id != 0xFFFFFFFF);

    return id;
}
