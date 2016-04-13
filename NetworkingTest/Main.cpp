
#include <iostream>
#include <cstring>
#include <string>
#include <cstdlib>

#include <SFML/Network.hpp>
#include <SFML/System.hpp>

#include "../src/engine/connection.hpp"

void printUsage()
{
    std::cout << "Please use flags \"-s\" for server or \"-c\" for client.\n";
    std::cout << "Note that the client flag also requires an ip address afterwards like:\n";
    std::cout << "\t-c <ip-address>\n";
    std::cout << "Note that both client and server require a port specifying the port of the server.\n";
    std::cout << "EXAMPLE:\n\t./NTest -s <server_port>\n\tOR\n";
    std::cout << "\t./NTest -c <server_ip> <server_port>\n";
}

void MainLoop(Connection& connection)
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
    if(argc < 3 || argc > 4)
    {
        printUsage();
        return 0;
    }

    bool isServer;
    sf::IpAddress address;
    unsigned short port;

    if(argc == 3 && std::strcmp("-s", argv[1]) == 0)
    {
        isServer = true;
        port = (unsigned short) std::strtoul(argv[2], 0, 10);
        if(port == 0)
        {
            std::cout << "ERROR: invalid port" << std::endl;
            printUsage();
            return 0;
        }
    }
    else if(argc == 4 && std::strcmp("-c", argv[1]) == 0)
    {
        isServer = false;
        address = sf::IpAddress(argv[2]);
        port = (unsigned short) std::strtoul(argv[3], 0, 10);
        if(port == 0)
        {
            std::cout << "ERROR: invalid port" << std::endl;
            printUsage();
            return 0;
        }
    }
    else
    {
        printUsage();
        return 0;
    }

    Connection connection(isServer ? Connection::SERVER : Connection::CLIENT, port);
    if(!isServer)
    {
        std::cout << "Expecting server at port " << port << std::endl;
        connection.connectToServer(address);
    }
    else
    {
        std::cout << "Starting server at port " << port << std::endl;
    }

    std::cout << "Ctrl+C to exit the program.\n";
    MainLoop(connection);
}
