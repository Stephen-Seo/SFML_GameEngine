
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
    std::cout << "Note that the client flag also requires an ip address afterwards.\n";
    std::cout << "Note that both client and server require a port specifying the port of the server.\n";
    std::cout << "The client can optionally broadcast by setting \"b\" instead of an IP address.\n";
    std::cout << "The client port can be specified, just append it to the options list.\n";
    std::cout << "EXAMPLE:\n\t./NTest -s <server_port>\n\tOR\n";
    std::cout << "\t./NTest -c <server_ip> <server_port>\n";
    std::cout << "\t./NTest -c b <server_port>\n";
    std::cout << "\t./NTest -c <server_ip> <server_port> <client_port>\n";
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
    if(argc < 3 || argc > 5)
    {
        printUsage();
        return 0;
    }

    bool isServer;
    sf::IpAddress address;
    unsigned short serverPort;
    unsigned short clientPort = 0;
    bool isBroadcast = false;

    if(argc == 3 && std::strcmp("-s", argv[1]) == 0)
    {
        isServer = true;
        serverPort = (unsigned short) std::strtoul(argv[2], 0, 10);
        if(serverPort == 0)
        {
            std::cout << "ERROR: invalid port" << std::endl;
            printUsage();
            return 0;
        }
    }
    else if((argc == 4 || argc == 5) && std::strcmp("-c", argv[1]) == 0)
    {
        isServer = false;
        if(std::strcmp("b", argv[2]) == 0)
        {
            isBroadcast = true;
        }
        else
        {
            address = sf::IpAddress(argv[2]);
        }
        serverPort = (unsigned short) std::strtoul(argv[3], 0, 10);
        if(argc == 5)
        {
            clientPort = (unsigned short) std::strtoul(argv[4], 0, 10);
            if(clientPort == 0)
            {
                std::cout << "ERROR: invalid client port" << std::endl;
                printUsage();
                return 0;
            }
            std::cout << "Client port manually set to " << clientPort << std::endl;
        }
        if(serverPort == 0)
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

    Connection connection(isServer ? Connection::SERVER : Connection::CLIENT, serverPort, clientPort, isBroadcast);
    if(!isServer)
    {
        std::cout << "Expecting server at port " << serverPort << std::endl;
        connection.connectToServer(address);
    }
    else
    {
        std::cout << "Starting server at port " << serverPort << std::endl;
    }

    std::cout << "Ctrl+C to exit the program.\n";
    MainLoop(connection);
}
