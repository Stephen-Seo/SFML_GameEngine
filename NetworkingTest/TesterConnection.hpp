
#ifndef TESTER_CONNECTION_HPP
#define TESTER_CONNECTION_HPP

#include "../src/engine/connection.hpp"

class TesterConnection : public Connection
{
public:
    TesterConnection();
    TesterConnection(Connection::Mode mode);

protected:
    virtual void receivedPacket(sf::Packet packet, sf::Uint32 address);
    virtual void connectionMade(sf::Uint32 address);
    virtual void connectionLost(sf::Uint32 address);

private:

};

#endif
