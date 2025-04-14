//
// Created by roussierenoa on 3/28/25.
//

#include <fcntl.h>
#include <poll.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

#include "Exception.hpp"
#include "NetworksUtils.hpp"
#include "Program.hpp"

#include "lib.hpp"

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
    this->_logger.log("Payload dataId: " +
        std::to_string(payload.dataId));
    this->_logger.log("Payload dataCount: "
        + std::to_string(payload.dataCount));
    for (int i = 0; i < nbrPayload; ++i) {
        msg.resize(sizeData);
        std::cout << "Data: " << i << std::endl;
        msg = this->_socket.readFromSocket(sizeData);
        for (const auto &byte : msg) {
            std::cout << std::hex << std::uppercase << std::setw(2)
                      << std::setfill('0') << static_cast<int>(byte) << " ";
        }
        std::cout << std::dec << std::endl;
        this->_handlePayload(msg, payload);
        msg.clear();
    }
}

void jetpack::Client::Program::_handlePayload(std::vector<unsigned char> msg,
    Payload_t payload) {
    if (payload.dataId == PayloadType_t::SIZE_T) {
        size_t value = 0;
        value |= static_cast<size_t>(msg[0]) << 24;
        value |= static_cast<size_t>(msg[1]) << 16;
        value |= static_cast<size_t>(msg[2]) << 8;
        value |= static_cast<size_t>(msg[3]);
        std::cout << "Converted size_t value: " << ntohl(value) << std::endl;
    }
}

void jetpack::Client::Program::_sniffANetwork() {
    while (this->_isOpen) {
        try {
            int socketFd = this->_socket.getSocketFd();

            if (socketFd == -1) {
                this->_graphic.serverError();
                try {
                    this->_connectToSocket(this->_ip, this->_port);
                    std::this_thread::sleep_for(std::chrono::seconds(2));
                } catch (...) {
                    std::this_thread::sleep_for(std::chrono::seconds(5));
                }
                continue;
            }
            struct pollfd pfd;
            pfd.fd = socketFd;
            pfd.events = POLLIN | POLLOUT;
            int pollResult = poll(&pfd, 1, 100);

            if (pollResult < 0) {
                this->_logger.log("Poll error");
                this->_socket.closeSocket();
                this->_graphic.serverError();
                continue;
            }
            this->_graphic.serverOK();

            if (pollResult > 0 && (pfd.revents & POLLOUT)) {
                this->_sendNewUsername();
                this->_sendPlayerInput();
            }
            if (pollResult > 0 && (pfd.revents & POLLIN)) {
                try {
                    std::cout << "DATA RECEIVED" << std::endl;
                    Header_t header{};
                    try {
                         header = getHeader(this->_logger, this->_socket);
                    } catch (HeaderException &) {
                        this->_socket.closeSocket();
                        this->_graphic.serverError();
                        continue;
                    }
                    if (header.nbrPayload == 0) {
                        this->_logger.log("No payload");
                        continue;
                    }
                    Payload_t payload {};
                    try {
                        payload = getPayload(this->_logger, this->_socket);
                    } catch (PayloadException &) {
                        this->_socket.closeSocket();
                        this->_graphic.serverError();
                        continue;
                    }
                    if (payload.dataId == PayloadType_t::START) {
                        this->_graphic.switchToGame();
                        continue;
                    }
                    this->_handleMessageFromServer(payload);
                    this->_graphic.serverOK();
                } catch (const Socket::SocketError &e) {
                    this->_logger.log("Socket read error: " +
                                      std::string(e.what()));
                    this->_graphic.serverError();
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
        if (this->_auth.isConnected() && !this->_isChangeUsername)
            return;
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
    : _logger(logger),
      _graphic(this->_sendUserInteraction, this->_changeUsername,
               this->_getUsername),
      _socket(AF_INET, SOCK_STREAM, 0) {
    this->_ip = ip;
    this->_port = port;
    this->_logger.log("Connecting to " + std::string(ip) +
                      " port: " + std::to_string(port));
    this->_connectToSocket(ip, port);
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
