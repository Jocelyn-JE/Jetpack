//
// Created by roussierenoa on 3/28/25.
//

#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <fcntl.h>
#include <string>
#include <poll.h>
#include "Program.hpp"
#include "Exception.hpp"
#include "lib.hpp"

void jetpack::Client::Program::_connnectToSocket(const char *ip,
    unsigned int port)
{
    this->_socket.resetSocket(AF_INET, SOCK_STREAM, 0);
    auto fd = (this->_socket.getSocketFd());
    fcntl(fd, F_SETFL, F_GETFL | O_NONBLOCK);
    try {
        this->_socket.connectSocket(ip, port);
        this->_socket.setCloseOnDestroy(true);
        this->_logger.log("Connected to server at " + std::string(ip) + ":" + std::to_string(port));
        this->_graphic.serverOK();
    } catch (const std::exception& e) {
        this->_graphic.serverError();
        this->_logger.log("Connection failed: " + std::string(e.what()));
    }
}

void jetpack::Client::Program::_handleMessageFromServer(std::string msg) {
	Header_t header;
	Payload_t payload;
	int nbrPayload = 0;
    int indexListCount = 2;

	if (msg.size() <= 4) {
        throw NetworkException("Message too small");
    }
	uint16_t dataHeader = (static_cast<uint8_t>(msg[0]) << 8) | static_cast<uint8_t>(msg[1]);
	header.rawData = ntohs(dataHeader);
    this->_logger.log("Received: littleEndian" + std::to_string(header.rawData));
    this->_logger.log("Received: " + std::to_string(dataHeader));
    this->_logger.log("magic1: " + std::to_string(header.magic1));
    this->_logger.log("magic2: " + std::to_string(header.magic2));
	if (!(header.magic1 == 42 && header.magic2 == 42)) {
        throw NetworkException("Message not valid no magic number");
    }
	nbrPayload = header.nbrPayload;
    for (int i = 0; i < nbrPayload; ++i) {
        uint16_t dataPayload = (static_cast<uint8_t>(msg[indexListCount]) << 8) | static_cast<uint8_t>(msg[indexListCount + 1]);
        payload.rawData = ntohs(dataPayload);
        this->_handlePayload(msg, payload, indexListCount);
        indexListCount += 2;
    }
}

void jetpack::Client::Program::_handlePayload(std::string msg, Payload_t payload, int &indexListCount) {
    if (payload.dataId == PayloadType_t::NAME) {
        if (payload.dataCount > 1)
            throw NetworkException("There is too much payload for this action expected 1 data currently: " + std::to_string(payload.dataCount));
        this->_interactionMutex.lock();
        this->_graphic.setUsername(msg.substr(indexListCount, 20));
        indexListCount += 20;
        this->_interactionMutex.unlock();
    }
}

void jetpack::Client::Program::_sniffANetwork() {
    while (this->_isOpen) {
        try {
            int socketFd = this->_socket.getSocketFd();

            if (socketFd == -1) {
                this->_graphic.serverError();
                try {
                    this->_connnectToSocket(this->_ip, this->_port);
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
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
                continue;
            }
            this->_graphic.serverOK();
            this->_sendPlayerInput();

            if (pollResult > 0 && (pfd.revents & POLLOUT)) {
                this->_sendPlayerInput();
            }
            if (pollResult > 0 && (pfd.revents & POLLIN)) {
                std::string buffer;
                try {
                    buffer = this->_socket.readFromSocket();
                    if (buffer.empty()) {
                        this->_logger.log("Server disconnected");
                        this->_graphic.serverError();
                        this->_socket.closeSocket();
                        continue;
                    }
                    this->_handleMessageFromServer(buffer);
                    this->_graphic.serverOK();
                } catch (const Socket::SocketError &e) {
                    this->_logger.log("Socket read error: " + std::string(e.what()));
                    this->_graphic.serverError();
                    this->_socket.closeSocket();
                } catch (const NetworkException &e) {
                    this->_logger.log("Network error: " + std::string(e.what()));
                }
            }
        } catch (const std::exception &e) {
            this->_logger.log("Error in network thread: " + std::string(e.what()));
            this->_graphic.serverError();
            try {
                this->_socket.closeSocket();
            } catch (...) {}
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }
}

void jetpack::Client::Program::_sendUpEvent() {
    if (this->_socket.getSocketFd() == -1) {
        this->_logger.log("Cannot send username: not connected to server");
        return;
    }
    Header_t header{};
    header.magic1 = 42;
    header.magic2 = 42;
    header.nbrPayload = 1;
    auto valueHeaderBigEndian = htons(header.rawData);
    this->_logger.log("Header: little endian: " +
                      std::to_string(header.rawData));
    this->_logger.log("Header: big endian: " +
                      std::to_string(valueHeaderBigEndian));
    Payload_t payload{};
    payload.dataCount = 1;
    payload.dataId = 13;
    auto valuePayloadBigEndian = htons(payload.rawData);
    this->_logger.log("Payload: little endian: " +
                      std::to_string(payload.rawData));
    this->_logger.log("Payload: big endian: " +
                      std::to_string(valuePayloadBigEndian));
    this->_socket.writeToSocket<unsigned short>(valueHeaderBigEndian);
    this->_socket.writeToSocket<unsigned short>(valuePayloadBigEndian);
    this->_socket.writeToSocket<bool>(true);
}

void jetpack::Client::Program::_sendPlayerInput() {
    this->_interactionMutex.lock();
    if (this->_lastUserInteraction == UserInteractions_s::NO_INTERACTION) {
        this->_interactionMutex.unlock();
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
        } catch (...) {}
    }
    this->_interactionMutex.unlock();
}

void jetpack::Client::Program::loop() {
    while (this->_isOpen) {
        if (!this->_graphic.isOpen())
            this->_isOpen = false;
        this->_graphic.analyse();
        this->_graphic.compute();
        this->_graphic.display();
    }
}

void jetpack::Client::Program::_sendChangeUsername() {
    try {
        if (this->_socket.getSocketFd() == -1) {
            this->_logger.log("Cannot send username: not connected to server");
            return;
        }
        Header_t header{};
        header.magic1 = 42;
        header.magic2 = 42;
        header.nbrPayload = 1;
        auto valueHeaderBigEndian = htons(header.rawData);
        this->_logger.log("Header: little endian: " +
                          std::to_string(header.rawData));
        this->_logger.log("Header: big endian: " +
                          std::to_string(valueHeaderBigEndian));
        Payload_t payload{};

        payload.dataCount = 1;
        payload.dataId = 8;
        auto valuePayloadBigEndian = htons(payload.rawData);

        this->_logger.log("Payload: little endian: " +
                          std::to_string(payload.rawData));
        this->_logger.log("Payload: big endian: " +
                          std::to_string(valuePayloadBigEndian));

        this->_socket.writeToSocket<unsigned short>(valueHeaderBigEndian);
        this->_socket.writeToSocket<unsigned short>(valuePayloadBigEndian);
        this->_socket.writeToSocket(this->_graphic.getUsername());
        for (size_t i = 0; i < 20 - this->_graphic.getUsername().length(); i++)
            this->_socket.writeToSocket('\0');
        this->_logger.log("Received Username: " + this->_graphic.getUsername());
    } catch (const Socket::SocketError &e) {
        std::cerr << "Error sending username change: " << e.what() << std::endl;
        try {
            this->_socket.closeSocket();
        } catch (...) {}
    }
}

jetpack::Client::Program::Program(const char *ip, unsigned int port,
                                  jetpack::Logger &logger)
    : _logger(logger),
      _graphic(this->_sendUserInteraction, this->_sendChangeUserName),
      _socket(AF_INET, SOCK_STREAM, 0)
{
    this->_ip = ip;
    this->_port = port;
    this->_logger.log("Connecting to " + std::string(ip) + " port: " + std::to_string(port));
    this->_connnectToSocket(ip, port);
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
    
    if (this->_networkThread.joinable())
        this->_networkThread.join();
}
