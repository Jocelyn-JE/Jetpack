//
// Created by roussierenoa on 3/28/25.
//

#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include "Program.hpp"
#include "Exception.hpp"
#include "lib.hpp"

void jetpack::Client::Program::_connnectToSocket(const char *ip,
                                                 unsigned int port) {
    this->_socket.connectSocket(ip, port);
    this->_socket.setCloseOnDestroy(true);
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
            std::string buffer = this->_socket.readFromSocket();
            if (!buffer.empty()) {
                this->_handleMessageFromServer(buffer);
            }
        } catch (const Socket::SocketError &e) {
            std::cerr << "Socket error: " << e.what() << std::endl;
            this->_isOpen = false;
        }
    }
    this->_interactionMutex.lock();
    this->_interactionMutex.unlock();
}

void jetpack::Client::Program::_sendPlayerInput(UserInteractions_s event) {
    this->_interactionMutex.lock();
    if (event == jetpack::Client::UP) {
        this->_socket.writeToSocket<std::string>("UP\n");
    }
    if (event == jetpack::Client::ESCAPE) {
        this->_socket.writeToSocket<std::string>("ESCAPE\n");
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
    this->_socharcket.writeToSocket(this->_graphic.getUsername());
    for (size_t i = 0; i < 20 - this->_graphic.getUsername().length(); i++)
        this->_socket.writeToSocket('\0');
    this->_logger.log("Received Username: " + this->_graphic.getUsername());
}

jetpack::Client::Program::Program(const char *ip, unsigned int port,
                                  jetpack::Logger &logger)
    : _logger(logger),
      _graphic(this->_sendUserInteraction, this->_sendChangeUserName),
      _socket(AF_INET, SOCK_STREAM, 0)
{
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
