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
#include "Packet.hpp"
#include "Player.hpp"
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
            if (server._clients.size() == 2 && !server._game->isStarted()) {
                server.sendToAllClients(server.createStartGamePacket());
                server._game->start(server._gameData->filename);
            }
            if (server._game->isStarted()) {
                server.sendToAllClients(server.createPlayerListPacket());
                server.sendToAllClients(server.createCoinListPacket());
            }
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
    std::vector<uint8_t> buffer;

    for (std::size_t i = 0; i < _socketPollList.size(); i++) {
        if (_socketPollList[i].revents & POLLIN && i == 0) {
            this->handleConnection();
            this->_game->addPlayer(_clients.back()->getId());
        }
        if (_socketPollList[i].revents & POLLIN && i != 0) {
            buffer = _clients[i - 1]->_controlSocket.readFromSocket(2);
            if (_clients[i - 1]->handlePayload(buffer)) {
                this->_game->delPlayer(_clients[i - 1]->getId());
                this->handleDisconnection(i);
            } else {
                // Data received from client, handle it here
                // std::cerr << "Payload received from client "
                //           << _clients[i - 1]->getId() << ": " << buffer
                //           << std::endl;
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

std::vector<uint8_t> jetpack::server::Server::createPlayerListPacket() {
    jetpack::server::Packet packet(1);
    packet.addPayloadHeader(_gameData->players.size(), PayloadType_t::PLAYER);
    for (const auto &player : _gameData->players) {
        packet.addData(player.second->id);
        packet.addData(player.second->username, 20);
        packet.addData(player.second->y_pos);
        packet.addData(player.second->coins_collected);
        packet.addData(player.second->is_dead);
        packet.addData(player.second->is_jetpack_on);
        packet.addData(player.second->host);
    }
    return packet.getPacket();
}

std::vector<uint8_t> jetpack::server::Server::createCoinListPacket() {
    jetpack::server::Packet packet(1);
    packet.addPayloadHeader(_gameData->coins.size(), PayloadType_t::COIN_POS);
    for (const auto &coin : _gameData->coins) {
        if (coin->x_pos < _gameData->advancement - 5 ||
            coin->x_pos > _gameData->advancement + 25)
            continue;
        packet.addData(coin->x_pos);
        packet.addData(coin->y_pos);
        packet.addData(coin->coinId);
    }
    return packet.getPacket();
}

std::vector<uint8_t> jetpack::server::Server::createConnectionPacket(
    size_t id, size_t gameSpeed) {
    jetpack::server::Packet packet(1);
    packet.add(std::vector<uint32_t>{static_cast<uint32_t>(id),
                                     static_cast<uint32_t>(gameSpeed)},
               PayloadType_t::SIZE_T);

    const auto &packetData = packet.getPacket();
    return packetData;
}

std::vector<uint8_t> jetpack::server::Server::createStartGamePacket(void) {
    jetpack::server::Packet packet(1);
    packet.addPayloadHeader(0, PayloadType_t::START);
    return packet.getPacket();
}
