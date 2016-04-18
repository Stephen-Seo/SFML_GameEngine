
#include "connection.hpp"

Connection::Connection(Mode mode, unsigned short serverPort, unsigned short clientPort, bool clientBroadcast) :
acceptNewConnections(true),
ignoreOutOfSequence(false),
resendTimedOutPackets(true),
mode(mode),
clientSentAddressSet(false),
initialized(false),
validState(false),
invalidNoticeTimer(INVALID_NOTICE_TIME),
serverPort(serverPort),
clientPort(clientPort),
clientRetryTimer(CLIENT_RETRY_TIME_SECONDS),
clientBroadcast(clientBroadcast)
{
}

void Connection::update(sf::Time dt)
{
    if(!initialized)
    {
#ifndef NDEBUG
        std::cout << "Lazy initializing connection..." << std::endl;
#endif
        initialize();
        initialized = true;
    }

    if(!validState)
    {
        invalidNoticeTimer -= dt.asSeconds();
        if(invalidNoticeTimer <= 0)
        {
            std::clog << "Warning: Connection is in invalid state, not doing anything!" << std::endl;
            invalidNoticeTimer = INVALID_NOTICE_TIME;
        }
        return;
    }

    for(auto iter = connectionData.begin(); iter != connectionData.end(); ++iter)
    {
        iter->second.toggleTimer += dt.asSeconds();
        iter->second.toggledTimer += dt.asSeconds();

        if(iter->second.isGood && !iter->second.isGoodRtt)
        {
            // good status, rtt is bad
#ifndef NDEBUG
            std::cout << "Switching to bad network mode for " << sf::IpAddress(iter->first).toString() << '\n';
#endif
            iter->second.isGood = false;
            if(iter->second.toggledTimer <= 10.0f)
            {
                iter->second.toggleTime *= 2.0f;
                if(iter->second.toggleTime > 60.0f)
                {
                    iter->second.toggleTime = 60.0f;
                }
            }
            iter->second.toggledTimer = 0.0f;
        }
        else if(iter->second.isGood)
        {
            // good status, rtt is good
            if(iter->second.toggleTimer >= 10.0f)
            {
                iter->second.toggleTimer = 0.0f;
                iter->second.toggleTime /= 2.0f;
                if(iter->second.toggleTime < 1.0f)
                {
                    iter->second.toggleTime = 1.0f;
                }
            }
        }
        else if(!iter->second.isGood && iter->second.isGoodRtt)
        {
            // bad status, rtt is good
            if(iter->second.toggledTimer >= iter->second.toggleTime)
            {
                iter->second.toggleTimer = 0.0f;
                iter->second.toggledTimer = 0.0f;
#ifndef NDEBUG
                std::cout << "Switching to good network mode for " << sf::IpAddress(iter->first).toString() << '\n';
#endif
                iter->second.isGood = true;
            }
        }
        else
        {
            // bad status, rtt is bad
            iter->second.toggledTimer = 0.0f;
        }

        iter->second.timer += dt.asSeconds();
        if(iter->second.timer >= (iter->second.isGood ? NETWORK_GOOD_MODE_SEND_INTERVAL : NETWORK_BAD_MODE_SEND_INTERVAL))
        {
            iter->second.timer = 0.0f;
            iter->second.triggerSend = true;
        }
    }

    if(mode == SERVER)
    {
        // check if clients have timed out
        std::list<sf::Uint32> disconnectQueue;
        for(auto iter = connectionData.begin(); iter != connectionData.end(); ++iter)
        {
            if(iter->second.elapsedTime.getElapsedTime().asMilliseconds() >= CONNECTION_TIMEOUT_MILLISECONDS)
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

        // send packet as server to each client
        for(auto iter = connectionData.begin(); iter != connectionData.end(); ++iter)
        {
            if(iter->second.triggerSend)
            {
                iter->second.triggerSend = false;
                if(!iter->second.sendPacketQueue.empty())
                {
                    auto pInfo = iter->second.sendPacketQueue.back();
                    iter->second.sendPacketQueue.pop_back();

                    sf::Packet toSendPacket;
                    sf::Uint32 sequenceID;
                    if(pInfo.isResending)
                    {
                        sequenceID = pInfo.ID;
                    }
                    else
                    {
                        preparePacket(toSendPacket, sequenceID, sf::IpAddress(iter->first));
                    }
                    const void* dataPtr = pInfo.packet.getData();
                    toSendPacket.append(dataPtr, pInfo.packet.getDataSize());

                    iter->second.sentPackets.push_front(PacketInfo(toSendPacket, iter->first, sequenceID));

                    socket.send(toSendPacket, sf::IpAddress(iter->first), iter->second.port);
                    checkSentPacketsSize(iter->first);
                }
                else
                {
                    // send a heartbeat(empty) packet because the queue is empty

                    sf::Packet toSendPacket;
                    sf::Uint32 sequenceID;
                    preparePacket(toSendPacket, sequenceID, sf::IpAddress(iter->first));
                    iter->second.sentPackets.push_front(PacketInfo(toSendPacket, iter->first, sequenceID));
                    socket.send(toSendPacket, sf::IpAddress(iter->first), iter->second.port);
                    checkSentPacketsSize(iter->first);
                }
            }
        }

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
                if(connectionData.find(address.toInteger()) == connectionData.end())
                {
#ifndef NDEBUG
                    std::cout << "SERVER: Establishing new connection with " << address.toString() << '\n';
#endif
                    // Establish connection
                    registerConnection(address.toInteger(), 0, remotePort);
                    sf::Packet newPacket;
                    sendPacket(newPacket, address);
                }
                return;
            }
            else if(ID == network::PING)
            {
                sf::Packet newPacket;
                sendPacket(newPacket, address);
            }
            else if(connectionData.find(address.toInteger()) == connectionData.end())
            {
                // Unknown client not attemping to connect, ignoring
                return;
            }
            else if(ID != connectionData.at(address.toInteger()).ID)
            {
                // ID and address doesn't match, ignoring
                return;
            }

            // packet is valid
#ifndef NDEBUG
            std::cout << "Valid packet " << sequence << " received from " << address.toString() << '\n';
#endif

            bool outOfOrder = false;

            sf::Uint32 clientAddress = address.toInteger();

            lookupRtt(clientAddress, ack);

            connectionData.at(clientAddress).elapsedTime.restart();
            checkSentPackets(ack, ackBitfield, clientAddress);

            sf::Uint32 diff = 0;
            if(sequence > connectionData.at(clientAddress).rSequence)
            {
                diff = sequence - connectionData.at(clientAddress).rSequence;
                if(diff <= 0x7FFFFFFF)
                {
                    // sequence is more recent
                    connectionData.at(clientAddress).rSequence = sequence;
                    shiftBitfield(address, diff);
                }
                else
                {
                    // sequence is older packet id, diff requires recalc
                    diff = sequence + (0xFFFFFFFF - connectionData.at(clientAddress).rSequence) + 1;

                    if((connectionData.at(clientAddress).ackBitfield & (0x100000000 >> diff)) != 0x0)
                    {
                        // already received packet
                        return;
                    }
                    connectionData.at(clientAddress).ackBitfield |= (0x100000000 >> diff);

                    if(ignoreOutOfSequence)
                        return;

                    outOfOrder = true;
                }
            }
            else if(connectionData.at(clientAddress).rSequence > sequence)
            {
                diff = connectionData.at(clientAddress).rSequence - sequence;
                if(diff > 0x7FFFFFFF)
                {
                    // sequence is more recent, diff requires recalc
                    diff = sequence + (0xFFFFFFFF - connectionData.at(clientAddress).rSequence) + 1;

                    connectionData.at(clientAddress).rSequence = sequence;
                    shiftBitfield(address, diff);
                }
                else
                {
                    // sequence is older packet id
                    if((connectionData.at(clientAddress).ackBitfield & (0x100000000 >> diff)) != 0x0)
                    {
                        // already received packet
                        return;
                    }
                    connectionData.at(clientAddress).ackBitfield |= (0x100000000 >> diff);

                    if(ignoreOutOfSequence)
                        return;

                    outOfOrder = true;
                }
            }
            else
            {
                // duplicate packet, ignoring
                return;
            }

            receivedPacket(packet, clientAddress, outOfOrder);
        }
    } // if(mode == SERVER)
    else if(mode == CLIENT)
    {
        // connection established
        if(connectionData.size() > 0)
        {
            sf::Uint32 serverAddress = clientSentAddress.toInteger();
            // check if timed out
            if(connectionData.at(serverAddress).elapsedTime.getElapsedTime().asMilliseconds() > CONNECTION_TIMEOUT_MILLISECONDS)
            {
#ifndef NDEBUG
                std::cout << "Disconnected from server " << clientSentAddress.toString() << '\n';
#endif
                unregisterConnection(serverAddress);
                return;
            }

            // send packet as client to server
            if(connectionData.at(serverAddress).triggerSend)
            {
                connectionData.at(serverAddress).triggerSend = false;
                if(!connectionData.at(serverAddress).sendPacketQueue.empty())
                {
                    PacketInfo pInfo = connectionData.at(serverAddress).sendPacketQueue.back();
                    connectionData.at(serverAddress).sendPacketQueue.pop_back();


                    sf::Packet toSendPacket;
                    sf::Uint32 sequenceID;
                    if(pInfo.isResending)
                    {
                        sequenceID = pInfo.ID;
                    }
                    else
                    {
                        preparePacket(toSendPacket, sequenceID, clientSentAddress);
                    }
                    const void* dataPtr = pInfo.packet.getData();
                    toSendPacket.append(dataPtr, pInfo.packet.getDataSize());

                    connectionData.at(serverAddress).sentPackets.push_front(PacketInfo(toSendPacket, serverAddress, sequenceID));

                    socket.send(toSendPacket, clientSentAddress, serverPort);
                    checkSentPacketsSize(serverAddress);
                }
                else
                {
                    // send a heartbeat(empty) packet because the queue is empty

                    sf::Packet toSendPacket;
                    sf::Uint32 sequenceID;
                    preparePacket(toSendPacket, sequenceID, clientSentAddress);
                    connectionData.at(serverAddress).sentPackets.push_front(PacketInfo(toSendPacket, serverAddress, sequenceID));
                    socket.send(toSendPacket, clientSentAddress, serverPort);
                    checkSentPacketsSize(clientSentAddress.toInteger());
                }
            }

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
                    sendPacket(newPacket, address);
                }
                else if(ID != connectionData.at(serverAddress).ID)
                    return;

                // packet is valid
#ifndef NDEBUG
                std::cout << "Valid packet " << sequence << " received from " << address.toString() << '\n';
#endif

                bool outOfOrder = false;

                lookupRtt(serverAddress, ack);

                connectionData.at(serverAddress).elapsedTime.restart();
                checkSentPackets(ack, bitfield, serverAddress);

                sf::Uint32 diff = 0;
                if(sequence > connectionData.at(serverAddress).rSequence)
                {
                    diff = sequence - connectionData.at(serverAddress).rSequence;
                    if(diff <= 0x7FFFFFFF)
                    {
                        // sequence is more recent
                        connectionData.at(serverAddress).rSequence = sequence;
                        shiftBitfield(address, diff);
                    }
                    else
                    {
                        // sequence is older packet id, diff requires recalc
                        diff = sequence + (0xFFFFFFFF - connectionData.at(serverAddress).rSequence) + 1;

                        if((connectionData.at(serverAddress).ackBitfield & (0x100000000 >> diff)) != 0x0)
                        {
                            // already received packet
                            return;
                        }
                        connectionData.at(serverAddress).ackBitfield |= (0x100000000 >> diff);

                        if(ignoreOutOfSequence)
                            return;

                        outOfOrder = true;
                    }
                }
                else if(connectionData.at(serverAddress).rSequence > sequence)
                {
                    diff = connectionData.at(serverAddress).rSequence - sequence;
                    if(diff > 0x7FFFFFFF)
                    {
                        // sequence is more recent, diff requires recalc
                        diff = sequence + (0xFFFFFFFF - connectionData.at(serverAddress).rSequence) + 1;

                        connectionData.at(serverAddress).rSequence = sequence;
                        shiftBitfield(address, diff);
                    }
                    else
                    {
                        // sequence is older packet id
                        if((connectionData.at(serverAddress).ackBitfield & (0x100000000 >> diff)) != 0x0)
                        {
                            // already received packet
                            return;
                        }
                        connectionData.at(serverAddress).ackBitfield |= (0x100000000 >> diff);

                        if(ignoreOutOfSequence)
                            return;

                        outOfOrder = true;
                    }
                }
                else
                {
                    // duplicate packet, ignoring
                    return;
                }

                receivedPacket(packet, serverAddress, outOfOrder);
            }
        }
        // connection not yet established
        else if(acceptNewConnections)
        {
            // check retry timer
            clientRetryTimer += dt.asSeconds();
            if(clientRetryTimer >= CLIENT_RETRY_TIME_SECONDS && (clientSentAddressSet || clientBroadcast))
            {
#ifndef NDEBUG
                std::cout << "CLIENT: Establishing connection with server..." << std::endl;
#endif
                clientRetryTimer = 0.0f;
                sf::Packet packet;
                packet << (sf::Uint32) GAME_PROTOCOL_ID << (sf::Uint32) network::CONNECT << (sf::Uint32) 0 << (sf::Uint32) 0 << (sf::Uint32) 0xFFFFFFFF;
                if(clientBroadcast)
                {
                    socket.send(packet, sf::IpAddress::Broadcast, serverPort);
                }
                else
                {
                    socket.send(packet, clientSentAddress, serverPort);
                }
            }

            // receive
            sf::Packet packet;
            sf::IpAddress address;
            unsigned short port;
            sf::Socket::Status status;
            status = socket.receive(packet, address, port);

            if(status == sf::Socket::Done && (address == clientSentAddress || clientBroadcast) && port == serverPort)
            {
#ifndef NDEBUG
                std::cout << "." << std::flush;
#endif
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

                if(clientBroadcast)
                {
                    clientSentAddress = address;
                    clientSentAddressSet = true;
                }
                registerConnection(address.toInteger(), ID, serverPort);
            }
        }
    } // elif(mode == CLIENT)
}

void Connection::connectToServer(sf::IpAddress address)
{
    if(mode != CLIENT)
        return;

#ifndef NDEBUG
    std::cout << "CLIENT: storing server ip as " << address.toString() << '\n';
#endif

    clientSentAddress = address;
    clientSentAddressSet = true;
}

void Connection::sendPacket(sf::Packet& packet, sf::IpAddress address)
{
    connectionData.at(address.toInteger()).sendPacketQueue.push_front(PacketInfo(packet, address.toInteger()));
}

sf::Time Connection::getRtt()
{
    if(connectionData.empty())
    {
        return sf::Time::Zero;
    }
    return connectionData.begin()->second.rtt;
}

sf::Time Connection::getRtt(sf::Uint32 address)
{
    if(connectionData.find(address) == connectionData.end())
    {
        return sf::Time::Zero;
    }
    return connectionData.at(address).rtt;
}

void Connection::setReceivedCallback(std::function<void(sf::Packet, sf::Uint32, bool)> callback)
{
    receivedCallback = callback;
}

void Connection::setConnectedCallback(std::function<void(sf::Uint32)> callback)
{
    connectedCallback = callback;
}

void Connection::setDisconnectedCallback(std::function<void(sf::Uint32)> callback)
{
    disconnectedCallback = callback;
}

std::list<sf::Uint32> Connection::getConnected()
{
    std::list<sf::Uint32> connectedList;

    for(auto iter = connectionData.begin(); iter != connectionData.end(); ++iter)
    {
        connectedList.push_back(iter->first);
    }

    return connectedList;
}

unsigned int Connection::getPacketQueueSize(sf::Uint32 destinationAddress)
{
    auto connectionDataIter = connectionData.find(destinationAddress);
    if(connectionDataIter == connectionData.end())
    {
        return 0;
    }

    return connectionDataIter->second.sendPacketQueue.size();
}

void Connection::clearPacketQueue(sf::Uint32 destinationAddress)
{
    auto connectionDataIter = connectionData.find(destinationAddress);
    if(connectionDataIter == connectionData.end())
    {
        return;
    }

    connectionDataIter->second.sendPacketQueue.clear();
}

bool Connection::connectionIsGood()
{
    auto connectionDataIter = connectionData.begin();
    if(connectionDataIter == connectionData.end())
    {
        return false;
    }

    return connectionDataIter->second.isGood;
}

bool Connection::connectionIsGood(sf::Uint32 destinationAddress)
{
    auto connectionDataIter = connectionData.find(destinationAddress);
    if(connectionDataIter == connectionData.end())
    {
        return false;
    }

    return connectionDataIter->second.isGood;
}

void Connection::reset(Connection::Mode mode, unsigned short serverPort, unsigned short clientPort, bool clientBroadcast)
{
    this->mode = mode;
    this->serverPort = serverPort;
    this->clientPort = clientPort;
    socket.unbind();
    connectionData.clear();
    clientSentAddressSet = false;
    initialized = false;
    validState = false;
    invalidNoticeTimer = INVALID_NOTICE_TIME;
    clientRetryTimer = CLIENT_RETRY_TIME_SECONDS;
    this->clientBroadcast = clientBroadcast;
}

void Connection::setClientBroadcast(bool clientWillBroadcast)
{
    clientBroadcast = clientWillBroadcast;
}

void Connection::registerConnection(sf::Uint32 address, sf::Uint32 ID, unsigned short port)
{
    if(mode == SERVER)
    {
        connectionData.insert(std::make_pair(address, ConnectionData(generateID(), 0, port)));
    }
    else if(mode == CLIENT)
    {
        connectionData.insert(std::make_pair(address, ConnectionData(ID, 1, port)));
    }

    connectionMade(address);
}

void Connection::unregisterConnection(sf::Uint32 address)
{
    connectionData.erase(address);

    connectionLost(address);
}

void Connection::shiftBitfield(sf::IpAddress address, sf::Uint32 diff)
{
    connectionData.at(address.toInteger()).ackBitfield = (connectionData.at(address.toInteger()).ackBitfield >> diff) | (0x100000000 >> diff);
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
        for(auto iter = connectionData.at(address).sentPackets.begin(); iter != connectionData.at(address).sentPackets.end(); ++iter)
        {
            if(iter->ID == ack)
            {
                // timed out, adding to send queue
                if(iter->sentTime.getElapsedTime() >= sf::milliseconds(PACKET_LOST_TIMEOUT_MILLISECONDS))
                {
#ifndef NDEBUG
                    std::cout << "Packet " << ack << "(" << std::hex << std::showbase << ack << std::dec;
                    std::cout << ") timed out\n";
#endif
                    sf::Packet packetCopy = iter->packet;
                    sf::Uint32 sequenceID;
                    packetCopy >> sequenceID; // protocol ID
                    packetCopy >> sequenceID; // ID of client
                    packetCopy >> sequenceID; // sequence ID
                    sendPacket(iter->packet, sf::IpAddress(address), sequenceID);
                    iter->sentTime.restart();
                }
                break;
            }
        }

        --ack;
    }
}

void Connection::lookupRtt(sf::Uint32 address, sf::Uint32 ack)
{
    for(auto iter = connectionData.at(address).sentPackets.begin(); iter != connectionData.at(address).sentPackets.end(); ++iter)
    {
        if(iter->ID == ack)
        {
            sf::Time time = iter->sentTime.getElapsedTime();
            if(time > connectionData.at(address).rtt)
            {
                connectionData.at(address).rtt += (time - connectionData.at(address).rtt) * 0.1f;
            }
            else
            {
                connectionData.at(address).rtt -= (connectionData.at(address).rtt - time) * 0.1f;
            }
#ifndef NDEBUG
            std::cout << "RTT of " << sf::IpAddress(address).toString() << " = " << connectionData.at(address).rtt.asMilliseconds() << '\n';
#endif
            connectionData.at(address).isGoodRtt = connectionData.at(address).rtt.asMilliseconds() <= 250;
            break;
        }
    }
}

void Connection::checkSentPacketsSize(sf::Uint32 address)
{
    while(connectionData.at(address).sentPackets.size() > SENT_PACKET_LIST_MAX_SIZE)
    {
        connectionData.at(address).sentPackets.pop_back();
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

void Connection::preparePacket(sf::Packet& packet, sf::Uint32& sequenceID, sf::IpAddress address, bool isPing)
{
    sf::Uint32 intAddress = address.toInteger();

    auto iter = connectionData.find(intAddress);
    assert(iter != connectionData.end());

    sf::Uint32 ID = iter->second.ID;

    sequenceID = (iter->second.lSequence)++;

    sf::Uint32 ack = iter->second.rSequence;

    sf::Uint32 ackBitfield = iter->second.ackBitfield;

    if(isPing)
    {
        packet << (sf::Uint32) GAME_PROTOCOL_ID << (sf::Uint32) network::PING << sequenceID << ack << ackBitfield;
    }
    else
    {
        packet << (sf::Uint32) GAME_PROTOCOL_ID << ID << sequenceID << ack << ackBitfield;
    }
}

void Connection::sendPacket(sf::Packet& packet, sf::IpAddress address, sf::Uint32 resendingID)
{
    connectionData.at(address.toInteger()).sendPacketQueue.push_front(PacketInfo(packet, address.toInteger(), resendingID, true));
}

void Connection::receivedPacket(sf::Packet packet, sf::Uint32 address, bool outOfOrder)
{
    if(receivedCallback && !packet.endOfPacket())
    {
        receivedCallback(packet, address, outOfOrder);
    }
}

void Connection::connectionMade(sf::Uint32 address)
{
    if(connectedCallback)
    {
        connectedCallback(address);
    }
}

void Connection::connectionLost(sf::Uint32 address)
{
    if(disconnectedCallback)
    {
        disconnectedCallback(address);
    }
}

void Connection::initialize()
{
    sf::Socket::Status sstatus;
    if(mode == CLIENT)
    {
        if(clientPort == 0)
        {
            sstatus = socket.bind(sf::Socket::AnyPort);
        }
        else
        {
            sstatus = socket.bind(clientPort);
        }
    }
    else
    {
        sstatus = socket.bind(serverPort);
    }

    switch(sstatus)
    {
    case sf::UdpSocket::Error:
        std::cerr << "ERROR: Binding socket to port " << socket.getLocalPort() << " returned error!" << std::endl;
        validState = false;
        return;
    case sf::UdpSocket::Done:
#ifndef NDEBUG
        std::cout << "Binding socket returned \"Done\"" << std::endl;
#endif
        break;
    case sf::UdpSocket::NotReady:
#ifndef NDEBUG
        std::cout << "Binding socket returned \"NotReady\"" << std::endl;
#endif
        break;
    case sf::UdpSocket::Partial:
#ifndef NDEBUG
        std::cout << "Binding socket returned \"Partial\"" << std::endl;
#endif
        break;
    case sf::UdpSocket::Disconnected:
#ifndef NDEBUG
        std::cout << "Binding socket returned \"Disconected\"" << std::endl;
#endif
        break;
    default:
        break;
    }

    validState = true;
    socket.setBlocking(false);
}

