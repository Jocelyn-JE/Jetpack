//
// Created by roussierenoa on 3/28/25.
//

#include <fcntl.h>
#include <poll.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>

#include "Coin.hpp"
#include "Exception.hpp"
#include "NetworksUtils.hpp"
#include "Obstacle.hpp"
#include "Player.hpp"
#include "Program.hpp"
#include "lib.hpp"

void jetpack::Client::Program::_setSize_tData(std::vector<unsigned char> msg) {
    if (!this->_auth.isConnected()) {
        size_t value = 0;
        value |= static_cast<size_t>(msg[0]) << 24;
        value |= static_cast<size_t>(msg[1]) << 16;
        value |= static_cast<size_t>(msg[2]) << 8;
        value |= static_cast<size_t>(msg[3]);
        this->_logger.log("User ID = " + std::to_string(ntohl(value)));
        this->_auth.setId(ntohl(value));
        return;
    }
    if (this->_auth.isConnected()) {
        size_t value = 0;
        value |= static_cast<size_t>(msg[0]) << 24;
        value |= static_cast<size_t>(msg[1]) << 16;
        value |= static_cast<size_t>(msg[2]) << 8;
        value |= static_cast<size_t>(msg[3]);
        this->_logger.log("GameSpeed = " + std::to_string(ntohl(value)));
        this->_graphic.setGameSpeed(ntohl(value));
        return;
    }
}

void jetpack::Client::Program::_setPlayerData(
    std::vector<unsigned char> msg) {
    if (msg.size() < 35) {
        this->_logger.log("Invalid player data size: "
                + std::to_string(msg.size()));
        return;
    }
    player_t player;
    uint32_t idInt = (msg[0] << 24) | (msg[1] << 16) | (msg[2] << 8) | msg[3];
    player.id = ntohl(idInt);
    std::string username;
    for (int i = 0; i < 20; i++) {
        if (msg[4 + i] != 0)
            username += static_cast<char>(msg[4 + i]);
        else
            break;
    }
    std::strncpy(player.username, username.c_str(),
                 sizeof(player.username) - 1);
    player.username[sizeof(player.username) - 1] = '\0';
    uint32_t yPosInt = (msg[25] << 24) | (msg[26] << 16)
        | (msg[27] << 8) | msg[28];
    yPosInt = ntohl(yPosInt);
    std::memcpy(&player.y_pos, &yPosInt, sizeof(player.y_pos));
    uint32_t coinsInt = (msg[29] << 24) | (msg[30] << 16)
        | (msg[31] << 8) | msg[32];
    player.coins_collected = ntohl(coinsInt);
    player.is_dead = msg[32] != 0;
    player.is_jetpack_on = msg[33] != 0;
    player.host = msg[34] != 0;
    this->_logger.log("Player ID: " + std::to_string(player.id));
    this->_logger.log("Player Username: " + std::string(player.username));
    this->_logger.log("Player Y Position: " + std::to_string(player.y_pos));
    this->_logger.log("Player Coins: " +
                      std::to_string(player.coins_collected));
    this->_graphic.addNewPlayer(player.id, this->_auth.getId() == player.id);
    this->_graphic.setPosPlayer(player.id, sf::Vector2f(0, player.y_pos * 40.f));
}

void jetpack::Client::Program::_setCoinData(std::vector<unsigned char> msg) {
    if (msg.size() < sizeof(coinsPos_s)) {
        this->_logger.log("Invalid coin data size: " +
                          std::to_string(msg.size()));
        return;
    }
    coinsPos_s coin;
    uint32_t xPosInt = (msg[0] << 24) | (msg[1] << 16) | (msg[2] << 8) | msg[3];
    coin.x_pos = ntohl(xPosInt);
    uint32_t yPosInt = (msg[4] << 24) | (msg[5] << 16) | (msg[6] << 8) | msg[7];
    coin.y_pos = ntohl(yPosInt);
    coin.coinId = (msg[8] << 24) | (msg[9] << 16) | (msg[10] << 8) | msg[11];
    coin.coinId = ntohl(coin.coinId);
    this->_logger.log("Coin Position: X=" + std::to_string(coin.x_pos) +
                      ", Y=" + std::to_string(coin.y_pos));
    this->_logger.log("Coin ID: " + std::to_string(coin.coinId));
    this->_graphic.addNewCoin(coin.coinId);
    this->_graphic.setPosCoin(coin.coinId,
                              sf::Vector2f(coin.x_pos, coin.y_pos));
}

void jetpack::Client::Program::_setLaserData(std::vector<unsigned char> msg) {
    if (msg.size() < sizeof(obstacle_t)) {
        this->_logger.log("Invalid obstacle data size: " +
                          std::to_string(msg.size()));
        return;
    }
    obstacle_t obstacle;
    uint32_t xPosInt = (msg[0] << 24) | (msg[1] << 16) | (msg[2] << 8) | msg[3];
    obstacle.x_pos = ntohl(xPosInt);
    uint32_t yPosInt = (msg[4] << 24) | (msg[5] << 16) | (msg[6] << 8) | msg[7];
    obstacle.y_pos = ntohl(yPosInt);
    int obstacleId = 0;
    if (msg.size() >= 12) {
        uint32_t idInt =
            (msg[8] << 24) | (msg[9] << 16) | (msg[10] << 8) | msg[11];
        obstacleId = ntohl(idInt);
    }
    this->_logger.log("Obstacle Position: X=" + std::to_string(obstacle.x_pos) +
                      ", Y=" + std::to_string(obstacle.y_pos));
    this->_logger.log("Obstacle ID: " + std::to_string(obstacleId));
    this->_graphic.addNewLaser(obstacleId);
    this->_graphic.setPosLaser(obstacleId,
                               sf::Vector2f(obstacle.x_pos, obstacle.y_pos));
}

void jetpack::Client::Program::_getServerMessage() {
    Header_t header{};
    try {
        header = getHeader(this->_logger, this->_socket);
    } catch (HeaderException &) {
        this->_socket.closeSocket();
        this->_graphic.serverError();
        this->_auth.resetAuth();
        throw GetMessageException("Header error");
    }
    if (header.nbrPayload == 0) {
        this->_logger.log("No payload");
        throw GetMessageException("Header error");
    }
    for (int i = 0; i < header.nbrPayload; ++i) {
        Payload_t payload{};
        try {
            payload = getPayload(this->_logger, this->_socket);
        } catch (PayloadException &) {
            this->_socket.closeSocket();
            this->_graphic.serverError();
            this->_auth.resetAuth();
            throw GetMessageException("Payload error");
        }
        if (payload.dataId == PayloadType_t::START)
            this->_graphic.switchToGame();
        else
            this->_handleMessageFromServer(payload);
    }
}

void jetpack::Client::Program::_connectToSocket(const char *ip,
                                                unsigned int port) {
    this->_socket.resetSocket(AF_INET, SOCK_STREAM, 0);
    auto fd = (this->_socket.getSocketFd());
    try {
        this->_socket.connectSocket(ip, port);
        this->_socket.setCloseOnDestroy(true);
        this->_logger.log("Connected to server at " + std::string(ip) + ":" +
                          std::to_string(port));
        this->_graphic.serverOK();
    } catch (const std::exception &e) {
        this->_graphic.serverError();
        this->_auth.resetAuth();
        this->_socket.closeSocket();
        this->_logger.log("Connection failed: " + std::string(e.what()));
    }
    fcntl(fd, F_SETFL, F_GETFL | O_NONBLOCK);
}

void jetpack::Client::Program::_handleMessageFromServer(Payload_t payload) {
    int nbrPayload = payload.dataCount;
    int sizeData = 0;
    std::vector<unsigned char> msg;
    sizeData = getPayloadSize(payload.dataId);
    this->_logger.log("Payload dataId: " + std::to_string(payload.dataId));
    this->_logger.log("Payload dataCount: " +
                      std::to_string(payload.dataCount));
    for (int i = 0; i < nbrPayload; ++i) {
        msg.resize(sizeData);
        this->_logger.log("Payload n°: " + std::to_string(i));
        msg = this->_socket.readFromSocket(sizeData);
        std::stringstream ss;
        for (const auto &byte : msg) {
            ss << std::hex << std::uppercase << std::setw(2)
               << std::setfill('0') << static_cast<int>(byte) << " ";
            ss << std::dec;
        }
        this->_logger.log(ss.str());
        this->_handlePayload(msg, payload);
        msg.clear();
    }
}

void jetpack::Client::Program::_handlePayload(std::vector<unsigned char> msg,
                                              Payload_t payload) {
    if (payload.dataId == SIZE_T) this->_setSize_tData(msg);
    if (payload.dataId == PLAYER) this->_setPlayerData(msg);
    if (payload.dataId == COIN_POS) this->_setCoinData(msg);
    if (payload.dataId == HAZARD_POS) this->_setLaserData(msg);
}

void jetpack::Client::Program::_sniffANetwork() {
    while (this->_isOpen) {
        try {
            int socketFd = this->_socket.getSocketFd();

            if (socketFd == -1 || this->_manualReco) {
                this->_manualReco = false;
                this->_socket.closeSocket();
                this->_graphic.serverError();
                this->_auth.resetAuth();
                try {
                    this->_connectToSocket(this->_ip.c_str(), this->_port);
                    std::this_thread::sleep_for(std::chrono::seconds(2));
                } catch (...) {
                    std::this_thread::sleep_for(std::chrono::seconds(5));
                }
                continue;
            }
            struct pollfd pfd;
            pfd.fd = socketFd;
            pfd.events = POLLIN;
            int pollResult = poll(&pfd, 1, 100);

            if (pollResult < 0) {
                this->_logger.log("Poll error");
                this->_socket.closeSocket();
                this->_graphic.serverError();
                this->_auth.resetAuth();
                continue;
            }
            this->_graphic.serverOK();

            if (pollResult > 0 && (pfd.revents & POLLOUT)) {
                this->_sendNewUsername();
                this->_sendPlayerInput();
            }
            if (pollResult > 0 && (pfd.revents & POLLIN)) {
                try {
                    try {
                        this->_getServerMessage();
                    } catch (GetMessageException &) {
                        continue;
                    }
                    this->_graphic.serverOK();
                } catch (const Socket::SocketError &e) {
                    this->_logger.log("Socket read error: " +
                                      std::string(e.what()));
                    this->_graphic.serverError();
                    this->_auth.resetAuth();
                    this->_socket.closeSocket();
                } catch (const NetworkException &e) {
                    this->_logger.log("Network error: " +
                                      std::string(e.what()));
                }
            }
        } catch (const std::exception &e) {
            this->_logger.log("Error in network thread: " +
                              std::string(e.what()));
            this->_graphic.serverError();
            this->_auth.resetAuth();
            try {
                this->_socket.closeSocket();
            } catch (...) {
            }
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }
}

void jetpack::Client::Program::_sendUpEvent() {
    if (this->_socket.getSocketFd() == -1) {
        this->_logger.log("Cannot send username: not connected to server");
        return;
    }
    Header_t header = generateHeader(1);
    auto valueHeaderBigEndian = htons(header.rawData);
    Payload_t payload = generatePayload(1, 13);
    auto valuePayloadBigEndian = htons(payload.rawData);
    this->_socket.writeToSocket<uint16_t>(valueHeaderBigEndian);
    this->_socket.writeToSocket<uint16_t>(valuePayloadBigEndian);
    this->_socket.writeToSocket<bool>(true);
}

void jetpack::Client::Program::_sendPlayerInput() {
    this->_communicationMutex.lock();
    this->_userInteractionMutex.lock();
    if (this->_lastUserInteraction == UserInteractions_s::NO_INTERACTION) {
        this->_userInteractionMutex.unlock();
        this->_communicationMutex.unlock();
        return;
    }
    try {
        if (this->_socket.getSocketFd() != -1) {
            if (this->_lastUserInteraction == jetpack::Client::UP) {
                this->_sendUpEvent();
            }
        }
        this->_lastUserInteraction = UserInteractions_s::NO_INTERACTION;
    } catch (const Socket::SocketError &e) {
        std::cerr << "Error sending player input: " << e.what() << std::endl;
        try {
            this->_socket.closeSocket();
        } catch (...) {
        }
    }
    this->_userInteractionMutex.unlock();
    this->_communicationMutex.unlock();
}

void jetpack::Client::Program::loop() {
    while (this->_isOpen) {
        if (!this->_graphic.isOpen()) this->_isOpen = false;
        this->_graphic.analyse();
        this->_graphic.compute();
        this->_graphic.display();
    }
}

void jetpack::Client::Program::_sendNewUsername() {
    try {
        if (this->_socket.getSocketFd() == -1) {
            this->_logger.log("Cannot send username: not connected to server");
            return;
        }
        if (this->_auth.isConnected() && !this->_isChangeUsername) return;
        Header_t header = generateHeader(1);
        auto valueHeaderBigEndian = htons(header.rawData);
        Payload_t payload = generatePayload(1, 8);
        auto valuePayloadBigEndian = htons(payload.rawData);
        this->_communicationMutex.lock();
        this->_usernameMutex.lock();
        this->_socket.writeToSocket<uint16_t>(valueHeaderBigEndian);
        this->_socket.writeToSocket<uint16_t>(valuePayloadBigEndian);
        this->_socket.writeToSocket(this->_getUsername());
        for (size_t i = 0; i < 20 - this->_getUsername().length(); i++)
            this->_socket.writeToSocket('\0');
        this->_logger.log("Sent Username: " + this->_getUsername());
        this->_isChangeUsername = false;
        this->_usernameMutex.unlock();
        this->_communicationMutex.unlock();
    } catch (const Socket::SocketError &e) {
        std::cerr << "Error sending username change: " << e.what() << std::endl;
        try {
            this->_socket.closeSocket();
        } catch (...) {
        }
    }
}

jetpack::Client::Program::Program(const char *ip, unsigned int port,
                                  jetpack::Logger &logger)
    : _ip(ip),
      _logger(logger),
      _graphic(this->_sendUserInteraction, this->_changeUsername,
               this->_getUsername, this->_getSocketSettings,
               this->_setSocketSettings, this->_getIdWithAuth,
               this->_getIsConnectedWithAuth),
      _socket(AF_INET, SOCK_STREAM, 0) {
    this->_port = port;
    this->_logger.log("Connecting to " + this->_ip +
                      " port: " + std::to_string(port));
    this->_portIpMutex.lock();
    this->_connectToSocket(this->_ip.c_str(), this->_port);
    this->_portIpMutex.unlock();
    this->_networkThread = std::thread([this] {
        pthread_setname_np(pthread_self(), "Network thread");
        try {
            this->_sniffANetwork();
        } catch (std::exception &e) {
            std::cerr << "Error: " << e.what() << std::endl;
            this->_isOpen = false;
        }
    });
}

jetpack::Client::Program::~Program() {
    this->_isOpen = false;

    if (this->_networkThread.joinable()) this->_networkThread.join();
}
