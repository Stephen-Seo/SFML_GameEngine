
#ifndef SERVER_HPP
#define SERVER_HPP

#include <list>
#include <map>

#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include "networkIdentifiers.hpp"

class Server
{
public:
    Server();
    virtual ~Server();

    void update(sf::Time dt);

    virtual void receivedPacket(sf::Packet packet);

protected:
    sf::Packet preparePacket(sf::IpAddress address);

    void sendPacket(sf::Packet& packet, sf::IpAddress address);

private:
    void registerClient(sf::IpAddress address);

    void heartbeat(sf::Uint32 addressInteger);

    sf::Uint32 generateID();


    sf::UdpSocket socket;
    std::map<sf::Uint32, sf::Time> elapsedTimeMap;

    std::random_device rd;
    std::uniform_int_distribution<sf::Uint32> dist;

    std::map<sf::Uint32, sf::Uint32> IDMap;

    std::map<sf::Uint32, sf::Uint32> lSequenceMap;
    std::map<sf::Uint32, sf::Uint32> rSequenceMap;

    std::map<sf::Uint32, sf::Uint32> rAckMap;
    std::map<sf::Uint32, sf::Uint32> ackBitfieldMap;

    std::map<sf::Uint32, std::list<std::pair<sf::Uint32, sf::Clock> > > missedAckQueueMap;
};

#endif
