/*
** EPITECH PROJECT, 2025
** my_jetpack
** File description:
** Server
*/

#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>

#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "CommunicationHeader.hpp"
#include "Server.hpp"
#include "Socket.hpp"

volatile sig_atomic_t stopFlag = 0;

static void handler(int signum) { stopFlag = signum; }

int jetpack::server::Server::runServer(Parser &parser) {
    int poll_result = 0;
    struct sigaction sa;

    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
    try {
        jetpack::server::Server server(parser);

        auto previousTime = std::chrono::high_resolution_clock::now();

        while (poll_result >= -1 && !stopFlag) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> deltaTimeDuration =
                currentTime - previousTime;
            server._game->update(deltaTimeDuration.count());
            previousTime = currentTime;
            poll_result = server.pollSockets();
            server.updateSockets();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return 0;
}

//-----------------------------------------------------------------------------
// The actual important stuff about communication is in this function
// The rest is helper functions / handling of connection and disconnection

void jetpack::server::Server::updateSockets() {
    std::string buffer;

    for (std::size_t i = 0; i < _socketPollList.size(); i++) {
        if (_socketPollList[i].revents & POLLIN && i == 0)
            this->handleConnection();
        if (_socketPollList[i].revents & POLLIN && i != 0) {
            buffer = _clients[i - 1]->_controlSocket.readFromSocket();
            if (_clients[i - 1]->handlePayload(buffer)) {
                this->handleDisconnection(i);
            } else {
                // Data received from client, handle it here
                std::cerr << "Payload received from client "
                          << _clients[i - 1]->getId() << ": " << buffer
                          << std::endl;
            }
        }
    }
}

//-----------------------------------------------------------------------------

jetpack::server::Server::Server(Parser &parser)
    : _serverSocket(AF_INET, SOCK_STREAM, 0),
      _socketPollList(_serverSocket.getSocketFd()),
      _nextClientId(0),
      _gameData(std::make_shared<GameData>()),
      _game(std::make_shared<Game>(_gameData)) {
    parser.parseServerFlags(*_gameData);
    _game->start(_gameData->filename);
    _serverSocket.bindSocket(_gameData->port);
    _serverSocket.listenSocket(LISTEN_BACKLOG);
    std::cerr << "Server started on port " << _gameData->port << std::endl;
}

jetpack::server::Server::~Server() {}

int jetpack::server::Server::pollSockets() {
    int result = poll(_socketPollList.data(), _clients.size() + 1, 1);
    if (result == -1) {
        _game->stop();
        throw Socket::SocketError("Poll failed: " +
                                  std::string(strerror(errno)));
    }
    return result;
}

bool jetpack::server::Server::isClosed() {
    return !_serverSocket.closesOnDestroy();
}

void jetpack::server::Server::handleDisconnection(int socketIndex) {
    _clients.erase(_clients.begin() + (socketIndex - 1));
    _socketPollList.removeSocket(_socketPollList[socketIndex].fd);
}

void jetpack::server::Server::handleConnection() {
    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    int client_socket =
        accept(_serverSocket.getSocketFd(), (struct sockaddr *)&client_addr,
               &client_addr_size);

    this->_clients.push_back(std::make_unique<Client>(
        client_socket, client_addr, this->_nextClientId));
    this->_nextClientId++;
    this->_socketPollList.addSocket(client_socket, POLLIN);
    std::cerr << inet_ntoa(client_addr.sin_addr) << ":"
              << ntohs(client_addr.sin_port)
              << " connected fd: " << client_socket << std::endl;
    this->_clients.back()->sendData(
        this->createConnectionPacket(_clients.back()->getId(), 1000));
}

jetpack::Header_t jetpack::server::Server::createPacketHeader(
    u_int8_t nbrPayload) {
    Header_t header;
    header.magic1 = 42;
    header.magic2 = 42;
    header.nbrPayload = nbrPayload;
    return header;
}

jetpack::Payload_t jetpack::server::Server::createPayloadHeader(
    u_int8_t dataCount, u_int8_t dataId) {
    Payload_t payload;
    payload.dataCount = dataCount;
    payload.dataId = dataId;
    return payload;
}

std::vector<uint8_t> jetpack::server::Server::createConnectionPacket(
    size_t id, size_t gameSpeed) {
    std::vector<uint8_t> packet;
    uint16_t headerBigEndian = htons(createPacketHeader(1).rawData);
    uint16_t payloadBigEndian = htons(createPayloadHeader(2, 2).rawData);
    size_t idBigEndian = htonl(id);
    size_t gameSpeedBigEndian = htonl(gameSpeed);

    packet.push_back(static_cast<uint8_t>(headerBigEndian >> 8));
    packet.push_back(static_cast<uint8_t>(headerBigEndian & 0xFF));
    packet.push_back(static_cast<uint8_t>(payloadBigEndian >> 8));
    packet.push_back(static_cast<uint8_t>(payloadBigEndian & 0xFF));
    packet.push_back(static_cast<uint8_t>(idBigEndian >> 24));
    packet.push_back(static_cast<uint8_t>(idBigEndian >> 16));
    packet.push_back(static_cast<uint8_t>(idBigEndian >> 8));
    packet.push_back(static_cast<uint8_t>(idBigEndian & 0xFF));
    packet.push_back(static_cast<uint8_t>(gameSpeedBigEndian >> 24));
    packet.push_back(static_cast<uint8_t>(gameSpeedBigEndian >> 16));
    packet.push_back(static_cast<uint8_t>(gameSpeedBigEndian >> 8));
    packet.push_back(static_cast<uint8_t>(gameSpeedBigEndian & 0xFF));
    std::cerr << "Packet created: " << std::endl;
    for (const auto &byte : packet) {
        std::cerr << std::hex << std::uppercase << std::setw(2)
                  << std::setfill('0') << static_cast<int>(byte) << " ";
    }
    std::cerr << std::dec << std::endl;
    return packet;
}

std::vector<uint8_t> jetpack::server::Server::createStartGamePacket(void) {
    std::vector<uint8_t> packet;
    uint16_t headerBigEndian = htons(createPacketHeader(1).rawData);
    uint16_t payloadBigEndian = htons(createPayloadHeader(0, 14).rawData);

    packet.push_back(static_cast<uint8_t>(headerBigEndian >> 8));
    packet.push_back(static_cast<uint8_t>(headerBigEndian & 0xFF));
    packet.push_back(static_cast<uint8_t>(payloadBigEndian >> 8));
    packet.push_back(static_cast<uint8_t>(payloadBigEndian & 0xFF));
    return packet;
}
