
#include "connection.hpp"

Connection::Connection() :
acceptNewConnections(true),
ignoreOutOfSequence(false),
resendTimedOutPackets(true),
mode(SERVER)
{
    socket.bind(GAME_PORT);
    socket.setBlocking(false);
}

Connection::Connection(Mode mode) :
acceptNewConnections(true),
ignoreOutOfSequence(false),
resendTimedOutPackets(true),
mode(mode)
{
    socket.bind(GAME_PORT);
    socket.setBlocking(false);
}

Connection::~Connection()
{
}

void Connection::update(sf::Time dt)
{
    if(mode == SERVER)
    {
        // check if clients have timed out
        std::list<sf::Uint32> disconnectQueue;
        for(auto iter = elapsedTimeMap.begin(); iter != elapsedTimeMap.end(); ++iter)
        {
            if(iter->second.getElapsedTime().asMilliseconds() >= CONNECTION_TIMEOUT_MILLISECONDS)
            {
                disconnectQueue.push_front(iter->first);
            }
        }

        for(auto iter = disconnectQueue.begin(); iter != disconnectQueue.end(); ++iter)
        {
#ifndef NDEBUG
            std::cout << "Disconnected " << sf::IpAddress(*iter).toString() << '\n';
#endif
            unregisterConnection(*iter);
        }

        // heartbeat packet
        for(auto tIter = heartbeatTimeMap.begin(); tIter != heartbeatTimeMap.end(); ++tIter)
        {
            tIter->second += dt;
            if(tIter->second.asMilliseconds() >= HEARTBEAT_MILLISECONDS)
            {
                tIter->second = sf::Time::Zero;
                heartbeat(tIter->first);
            }
        }

        // send packet
        if(!sendPacketQueue.empty())
        {
            auto pInfo = sendPacketQueue.back();
            sendPacketQueue.pop_back();

            heartbeatTimeMap[pInfo.address] = sf::Time::Zero;

            sentPackets[pInfo.address].push_front(PacketInfo(pInfo.packet, pInfo.ID));
            socket.send(pInfo.packet, sf::IpAddress(pInfo.address), GAME_PORT);
            checkSentPacketsSize(pInfo.address);
        }
        //TODO flow control


        // receive packet
        sf::Packet packet;
        sf::IpAddress address;
        unsigned short remotePort;
        sf::Socket::Status status = socket.receive(packet, address, remotePort);
        if(status == sf::Socket::Done)
        {
            sf::Uint32 protocolID;
            if(!(packet >> protocolID))
                return;

            // check protocol ID
            if(protocolID != GAME_PROTOCOL_ID)
                return;

            sf::Uint32 ID;
            sf::Uint32 sequence;
            sf::Uint32 ack;
            sf::Uint32 ackBitfield;

            if(!(packet >> ID) || !(packet >> sequence) || !(packet >> ack) || !(packet >> ackBitfield))
                return;

            if(ID == network::CONNECT && acceptNewConnections)
            {
                if(IDMap.find(address.toInteger()) == IDMap.end())
                {
#ifndef NDEBUG
                    std::cout << "SERVER: Establishing new connection with " << address.toString() << '\n';
                    std::cout << "\taddress->int = " << address.toInteger() << '\n';
#endif
                    // Establish connection
                    registerConnection(address.toInteger());
                    sf::Packet newPacket;
                    sf::Uint32 sequenceID;
                    preparePacket(newPacket, sequenceID, address);
                    sendPacket(newPacket, sequenceID, address);
                }
                return;
            }
            else if(ID == network::PING)
            {
                sf::Packet newPacket;
                sf::Uint32 sequenceID;
                preparePacket(newPacket, sequenceID, address);
                sendPacket(newPacket, sequenceID, address);
            }
            else if(IDMap.find(address.toInteger()) == IDMap.end())
            {
                // Unknown client not attemping to connect, ignoring
                return;
            }
            else if(ID != IDMap[address.toInteger()])
            {
                // ID and address doesn't match, ignoring
                return;
            }

            // packet is valid
#ifndef NDEBUG
            std::cout << "Valid packet received from " << address.toString() << '\n';
#endif

            sf::Uint32 clientAddress = address.toInteger();

            lookupRtt(clientAddress, ack);

            elapsedTimeMap[clientAddress].restart();
            checkSentPackets(ack, ackBitfield, clientAddress);

            sf::Uint32 diff = 0;
            if(sequence > rSequenceMap[clientAddress])
            {
                diff = sequence - rSequenceMap[clientAddress];
                if(diff <= 0x7FFFFFFF)
                {
                    // sequence is more recent
                    rSequenceMap[clientAddress] = sequence;
                    shiftBitfield(address, diff);
                }
                else
                {
                    // sequence is older packet id, diff requires recalc
                    diff = sequence + (0xFFFFFFFF - rSequenceMap[clientAddress]) + 1;

                    if((ackBitfieldMap[clientAddress] & (0x100000000 >> diff)) != 0x0)
                    {
                        // already received packet
                        return;
                    }
                    ackBitfieldMap[clientAddress] |= (0x100000000 >> diff);

                    if(ignoreOutOfSequence)
                        return;
                }
            }
            else if(rSequenceMap[clientAddress] > sequence)
            {
                diff = rSequenceMap[clientAddress] - sequence;
                if(diff > 0x7FFFFFFF)
                {
                    // sequence is more recent, diff requires recalc
                    diff = sequence + (0xFFFFFFFF - rSequenceMap[clientAddress]) + 1;

                    rSequenceMap[clientAddress] = sequence;
                    shiftBitfield(address, diff);
                }
                else
                {
                    // sequence is older packet id
                    if((ackBitfieldMap[clientAddress] & (0x100000000 >> diff)) != 0x0)
                    {
                        // already received packet
                        return;
                    }
                    ackBitfieldMap[clientAddress] |= (0x100000000 >> diff);

                    if(ignoreOutOfSequence)
                        return;
                }
            }
            else
            {
                // duplicate packet, ignoring
                return;
            }

            receivedPacket(packet, clientAddress);
        }
    } // if(mode == SERVER)
    else if(mode == CLIENT)
    {
        // connection established
        if(IDMap.size() > 0)
        {
            sf::Uint32 serverAddress = clientSentAddress.toInteger();
            // check if timed out
            sf::Time elapsedTime = elapsedTimeMap[serverAddress].getElapsedTime();
            if(elapsedTime.asMilliseconds() > CONNECTION_TIMEOUT_MILLISECONDS)
            {
#ifndef NDEBUG
                std::cout << "Disconnected from server " << clientSentAddress.toString() << '\n';
#endif
                unregisterConnection(serverAddress);
                return;
            }

            // heartbeat
            heartbeatTimeMap[serverAddress] += dt;
            if(heartbeatTimeMap[serverAddress].asMilliseconds() >= HEARTBEAT_MILLISECONDS)
            {
                heartbeatTimeMap[serverAddress] = sf::Time::Zero;
                heartbeat(serverAddress);
            }

            // send
            if(!sendPacketQueue.empty())
            {
                PacketInfo pInfo = sendPacketQueue.back();
                sendPacketQueue.pop_back();

                heartbeatTimeMap[pInfo.address] = sf::Time::Zero;

                sentPackets[serverAddress].push_front(PacketInfo(pInfo.packet, pInfo.ID));
                socket.send(pInfo.packet, sf::IpAddress(pInfo.address), GAME_PORT);
                checkSentPacketsSize(serverAddress);
            }

            //TODO flow control

            // receive
            sf::Packet packet;
            sf::IpAddress address;
            unsigned short port;
            sf::Socket::Status status;
            status = socket.receive(packet, address, port);

            if(status == sf::Socket::Done && address == clientSentAddress)
            {
                sf::Uint32 protocolID;
                if(!(packet >> protocolID))
                    return;

                if(protocolID != GAME_PROTOCOL_ID)
                    return;

                sf::Uint32 ID;
                sf::Uint32 sequence;
                sf::Uint32 ack;
                sf::Uint32 bitfield;

                if(!(packet >> ID) || !(packet >> sequence) || !(packet >> ack) || !(packet >> bitfield))
                    return;

                if(ID == network::PING)
                {
                    sf::Packet newPacket;
                    sf::Uint32 sequenceID;
                    preparePacket(newPacket, sequenceID, address);
                    sendPacket(newPacket, sequenceID, address);
                }
                else if(ID != IDMap[serverAddress])
                    return;

                // packet is valid
#ifndef NDEBUG
                std::cout << "Valid packet received from " << address.toString() << '\n';
#endif

                lookupRtt(serverAddress, ack);

                elapsedTimeMap[serverAddress].restart();
                checkSentPackets(ack, bitfield, serverAddress);

                sf::Uint32 diff = 0;
                if(sequence > rSequenceMap[serverAddress])
                {
                    diff = sequence - rSequenceMap[serverAddress];
                    if(diff <= 0x7FFFFFFF)
                    {
                        // sequence is more recent
                        rSequenceMap[serverAddress] = sequence;
                        shiftBitfield(address, diff);
                    }
                    else
                    {
                        // sequence is older packet id, diff requires recalc
                        diff = sequence + (0xFFFFFFFF - rSequenceMap[serverAddress]) + 1;

                        if((ackBitfieldMap[serverAddress] & (0x100000000 >> diff)) != 0x0)
                        {
                            // already received packet
                            return;
                        }
                        ackBitfieldMap[serverAddress] |= (0x100000000 >> diff);

                        if(ignoreOutOfSequence)
                            return;
                    }
                }
                else if(rSequenceMap[serverAddress] > sequence)
                {
                    diff = rSequenceMap[serverAddress] - sequence;
                    if(diff > 0x7FFFFFFF)
                    {
                        // sequence is more recent, diff requires recalc
                        diff = sequence + (0xFFFFFFFF - rSequenceMap[serverAddress]) + 1;

                        rSequenceMap[serverAddress] = sequence;
                        shiftBitfield(address, diff);
                    }
                    else
                    {
                        // sequence is older packet id
                        if((ackBitfieldMap[serverAddress] & (0x100000000 >> diff)) != 0x0)
                        {
                            // already received packet
                            return;
                        }
                        ackBitfieldMap[serverAddress] |= (0x100000000 >> diff);

                        if(ignoreOutOfSequence)
                            return;
                    }
                }
                else
                {
                    // duplicate packet, ignoring
                    return;
                }

                receivedPacket(packet, serverAddress);
            }
        }
        // connection not yet established
        else if(acceptNewConnections)
        {
            // receive
            sf::Packet packet;
            sf::IpAddress address;
            unsigned short port;
            sf::Socket::Status status;
            status = socket.receive(packet, address, port);

            if(status == sf::Socket::Done && address == clientSentAddress)
            {
                sf::Uint32 protocolID;
                if(!(packet >> protocolID))
                    return;

                if(protocolID != GAME_PROTOCOL_ID)
                    return;

                sf::Uint32 ID;
                sf::Uint32 sequence;
                sf::Uint32 ack;
                sf::Uint32 bitfield;

                if(!(packet >> ID) || !(packet >> sequence) || !(packet >> ack) || !(packet >> bitfield))
                    return;

                registerConnection(address.toInteger(), ID);
            }
        }
    } // elif(mode == CLIENT)
}

void Connection::connectToServer(sf::IpAddress address)
{
    if(mode != CLIENT)
        return;
#ifndef NDEBUG
    std::cout << "CLIENT: sending connection request to server at " << address.toString() << '\n';
#endif
    sf::Packet packet;
    packet << (sf::Uint32) GAME_PROTOCOL_ID << (sf::Uint32) network::CONNECT << (sf::Uint32) 0 << (sf::Uint32) 0 << (sf::Uint32) 0xFFFFFFFF;
    socket.send(packet, address, GAME_PORT);
    clientSentAddress = address;
}

void Connection::preparePacket(sf::Packet& packet, sf::Uint32& sequenceID, sf::IpAddress address, bool isPing)
{
    sf::Uint32 intAddress = address.toInteger();

    auto iter = IDMap.find(intAddress);
    assert(iter != IDMap.end());

    sf::Uint32 ID = iter->second;

    sequenceID = lSequenceMap[intAddress]++;

    sf::Uint32 ack = rSequenceMap[intAddress];

    sf::Uint32 ackBitfield = ackBitfieldMap[intAddress];

    if(isPing)
    {
        packet << (sf::Uint32) GAME_PROTOCOL_ID << (sf::Uint32) network::PING << sequenceID << ack << ackBitfield;
    }
    else
    {
        packet << (sf::Uint32) GAME_PROTOCOL_ID << ID << sequenceID << ack << ackBitfield;
    }
}

void Connection::sendPacket(sf::Packet& packet, sf::Uint32 sequenceID, sf::IpAddress address)
{
    sendPacketQueue.push_front(PacketInfo(packet, sequenceID, address.toInteger()));
}

void Connection::receivedPacket(sf::Packet packet, sf::Uint32 address)
{}

void Connection::connectionMade(sf::Uint32 address)
{}

void Connection::connectionLost(sf::Uint32 address)
{}

void Connection::registerConnection(sf::Uint32 address, sf::Uint32 ID)
{
    heartbeatTimeMap.insert(std::make_pair(address, sf::Time()));
    elapsedTimeMap.insert(std::make_pair(address, sf::Clock()));

    if(mode == SERVER)
    {
        IDMap.insert(std::make_pair(address, generateID()));
        lSequenceMap.insert(std::make_pair(address, (sf::Uint32) 0));
    }
    else if(mode == CLIENT)
    {
        IDMap.insert(std::make_pair(address, ID));
        lSequenceMap.insert(std::make_pair(address, (sf::Uint32) 1));
    }

    rSequenceMap.insert(std::make_pair(address, (sf::Uint32) 0));

    ackBitfieldMap.insert(std::make_pair(address, (sf::Uint32) 0xFFFFFFFF));

    sentPackets.insert(std::make_pair(address, std::list<PacketInfo>()));

    rttMap.insert(std::make_pair(address, sf::Time()));

    connectionMade(address);
}

void Connection::unregisterConnection(sf::Uint32 address)
{
    heartbeatTimeMap.erase(address);
    elapsedTimeMap.erase(address);

    IDMap.erase(address);

    lSequenceMap.erase(address);
    rSequenceMap.erase(address);

    ackBitfieldMap.erase(address);

    sentPackets.erase(address);

    rttMap.erase(address);

    connectionLost(address);
}

void Connection::shiftBitfield(sf::IpAddress address, sf::Uint32 diff)
{
    ackBitfieldMap[address.toInteger()] = (ackBitfieldMap[address.toInteger()] >> diff) | (0x100000000 >> diff);
}

void Connection::checkSentPackets(sf::Uint32 ack, sf::Uint32 bitfield, sf::Uint32 address)
{
    if(!resendTimedOutPackets)
        return;

    --ack;
    for(; bitfield != 0x0; bitfield = bitfield << 1)
    {
        // if received, don't bother checking
        if((0x80000000 & bitfield) != 0x0)
        {
            --ack;
            continue;
        }

        // not received by client yet, checking if packet timed out
        for(auto iter = sentPackets[address].begin(); iter != sentPackets[address].end(); ++iter)
        {
            if(iter->ID == ack)
            {
                // timed out, adding to send queue
                if(iter->sentTime.getElapsedTime() >= sf::milliseconds(PACKET_LOST_TIMEOUT_MILLISECONDS))
                {
#ifndef NDEBUG
                    std::cout << "Packet " << std::hex << std::showbase << ack << std::dec;
                    std::cout << " timed out\n";
#endif
                    sf::Packet packetCopy = iter->packet;
                    sf::Uint32 sequenceID;
                    packetCopy >> sequenceID; // protocol ID
                    packetCopy >> sequenceID; // ID of client
                    packetCopy >> sequenceID; // sequence ID
                    sendPacket(iter->packet, sequenceID, sf::IpAddress(address));
                    iter->sentTime.restart();
                }
                break;
            }
        }

        --ack;
    }
}

void Connection::heartbeat()
{
    for(auto iter = IDMap.begin(); iter != IDMap.end(); ++iter)
    {
        heartbeat(iter->first);
    }
}

void Connection::heartbeat(sf::Uint32 addressInteger)
{
    sf::IpAddress address(addressInteger);

    sf::Packet packet;
    sf::Uint32 sequenceID;
    preparePacket(packet, sequenceID, address);
    sendPacket(packet, sequenceID, address);
}

void Connection::lookupRtt(sf::Uint32 address, sf::Uint32 ack)
{
    for(auto iter = sentPackets[address].begin(); iter != sentPackets[address].end(); ++iter)
    {
        if(iter->ID == ack)
        {
            sf::Time time = iter->sentTime.getElapsedTime();
            if(time > rttMap[address])
            {
                rttMap[address] += (time - rttMap[address]) * 0.1f;
            }
            else
            {
                rttMap[address] -= (rttMap[address] - time) * 0.1f;
            }
#ifndef NDEBUG
            std::cout << "RTT of " << sf::IpAddress(address).toString() << " = " << rttMap[address].asMilliseconds() << '\n';
#endif
            break;
        }
    }
}

void Connection::checkSentPacketsSize(sf::Uint32 address)
{
    while(sentPackets[address].size() > SENT_PACKET_LIST_MAX_SIZE)
    {
        sentPackets[address].pop_back();
    }
}

sf::Uint32 Connection::generateID()
{
    sf::Uint32 id;
    do
    {
        id = dist(rd);
    } while (network::isSpecialID(id));

    return id;
}
