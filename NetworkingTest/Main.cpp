
#include <iostream>
#include <cstring>
#include <string>

#include <SFML/Network.hpp>
#include <SFML/System.hpp>

#include "TesterConnection.hpp"

void printUsage()
{
    std::cout << "Please use flags \"-s\" for server or \"-c\" for client.\n";
    std::cout << "Note that the client flag also requires an ip address afterwards like:\n";
    std::cout << "\t-c <ip-address>\n";
}

void MainLoop(TesterConnection& connection)
{
    sf::Clock clock;
    sf::Time lastUpdateTime = sf::Time::Zero;
    sf::Time frameTime = sf::seconds(1.0f / 60.0f);
    while(true)
    {
        lastUpdateTime += clock.restart();
        while(lastUpdateTime >= frameTime)
        {
            lastUpdateTime -= frameTime;
            connection.update(frameTime);
        }
    }
}

int main(int argc, char** argv)
{
    if(argc < 2 || argc > 3)
    {
        printUsage();
        return 0;
    }

    bool isServer;
    sf::IpAddress address;

    if(std::strcmp("-s", argv[1]) == 0)
    {
        isServer = true;
    }
    else if(argc == 3 && std::strcmp("-c", argv[1]) == 0)
    {
        isServer = false;
        address = sf::IpAddress(argv[2]);
    }
    else
    {
        printUsage();
        return 0;
    }

    TesterConnection connection(isServer ? Connection::SERVER : Connection::CLIENT);
    if(!isServer)
    {
        connection.connectToServer(address);
    }

    std::cout << "Ctrl+C to exit the program.";
    MainLoop(connection);
}
