
#include "TesterConnection.hpp"

#include <iostream>

TesterConnection::TesterConnection() :
Connection()
{}

TesterConnection::TesterConnection(Connection::Mode mode) :
Connection(mode)
{}

void TesterConnection::receivedPacket(sf::Packet packet, sf::Uint32 address)
{
    sf::IpAddress theAddress(address);

    std::cout << "Received packet of size " << packet.getDataSize() << " from " << theAddress.toString();
    std::cout << " rtt of " << getRtt(address).asSeconds() << '\n';
}

void TesterConnection::connectionMade(sf::Uint32 address)
{
    sf::IpAddress theAddress(address);

    std::cout << "Connection established with " << theAddress.toString() << '\n';
}

void TesterConnection::connectionLost(sf::Uint32 address)
{
    sf::IpAddress theAddress(address);

    std::cout << "Connection lost with " << theAddress.toString() << '\n';
}

