//
// Created by roussierenoa on 3/28/25.
//

#include "Program.hpp"
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include "Exception.hpp"
#include "communicationHeader.hpp"
#include "lib.hpp"

void jetpack::Client::Program::_connnectToSocket(const char *ip,
                                                 unsigned int port) {
	struct sockaddr_in data{};
	this->_socketFd = socket(AF_INET, SOCK_STREAM, 0);

	if (this->_socketFd == -1) return;
	data.sin_family = AF_INET;
	data.sin_port = htons(port);
	data.sin_addr.s_addr = inet_addr(ip);
	if (connect(this->_socketFd, reinterpret_cast<struct sockaddr *>(&data),
	            sizeof(data)) == -1) {
		perror("connect()");
		close(this->_socketFd);
	}
}

void jetpack::Client::Program::_handleMessageFromServer(std::string msg) {
	if (msg.starts_with("POS") && msg.ends_with('\n')) {
		msg.erase(msg.length() - 1);
		auto data = splitSentence(msg);
		if (data.size() != 4) return;
		this->_graphic.setPosRectangle(
			atoi(data[1].c_str()),
			sf::Vector2f{
				static_cast<float>(atof(data[2].c_str())),
				static_cast<float>(atof(data[3].c_str()))
			});
	}
	if (msg.starts_with("INIT") && msg.ends_with('\n')) {
		msg.erase(msg.length() - 1);
		auto data = splitSentence(msg, ':');
		if (data.size() != 3) return;
		this->_graphic.addNewPlayer(atoi(data[1].c_str()),
		                            atoi(data[2].c_str()));
	}
	if (msg.starts_with("RESET") && msg.ends_with('\n')) {
		msg.erase(msg.length() - 1);
		auto data = splitSentence(msg, ':');
		if (data.size() != 2) return;
		this->_graphic.setPosRectangle(atoi(data[1].c_str()),
		                               sf::Vector2f{0.0f, 0.0f});
	}
}

void jetpack::Client::Program::_sniffANetwork() {
	while (this->_isOpen) {
		char buffer[1024] = {0};
		this->_interactionMutex.lock();
		int bytesReceived =
				recv(this->_socketFd, buffer, sizeof(buffer) - 1, MSG_DONTWAIT);
		this->_interactionMutex.unlock();
		if (bytesReceived < 0) {
			if (errno == EWOULDBLOCK || errno == EAGAIN) {
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				continue;
			}
			perror("recv()");
			this->_isOpen = false;
			break;
		}
		if (bytesReceived == 0) {
			std::cout << "Server closed the connection." << std::endl;
			this->_isOpen = false;
			break;
		}
		this->_handleMessageFromServer(std::string(buffer, bytesReceived));
	}
	this->_interactionMutex.lock();
	close(this->_socketFd);
	this->_interactionMutex.unlock();
}

void jetpack::Client::Program::_sendPlayerInput(UserInteractions_s event) {
	this->_interactionMutex.lock();
	if (event == jetpack::Client::UP) {
		if (write(this->_socketFd, "UP\n", 3) == -1) {
			perror("write()");
		}
	}
	if (event == jetpack::Client::ESCAPE) {
		if (write(this->_socketFd, "ESPACE\n", 7) == -1) {
			perror("write()");
		}
	}
	this->_interactionMutex.unlock();
}

void jetpack::Client::Program::loop() {
	while (this->_isOpen) {
		if (!this->_graphic.isOpen()) this->_isOpen = false;
		this->_graphic.handleEvent();
		this->_graphic.compute();
		this->_graphic.display();
	}
}

jetpack::Client::Program::Program(const char *ip, unsigned int port,
                                  jetpack::Logger &logger)
	: _logger(logger), _graphic(_sendUserInteraction) {
	this->_logger.log("Connecting to " + std::string(ip) +
	                  " port: " + std::to_string(port));
	this->_connnectToSocket(ip, port);
	if (this->_socketFd == -1) {
		throw NetworkException("Failed to init socket");
	}
	Header_t header{};

	header.magic1 = 42;
	header.magic2 = 42;
	header.nbrPayload = 1;
	auto valueHeaderBigEndian = htons(header.rawData);
	this->_logger.log("Header: little endian: " + std::to_string(header.rawData));
	this->_logger.log("Header: big endian: " + std::to_string(valueHeaderBigEndian));
	Payload_t payload{};

	payload.dataCount = 1;
	payload.dataId = 8;
	auto valuePayloadBigEndian = htons(payload.rawData);

	this->_logger.log("Payload: little endian: " + std::to_string(payload.rawData));
	this->_logger.log("Payload: big endian: " + std::to_string(valuePayloadBigEndian));
	// unsigned char username[20] = "";

	write(this->_socketFd, &valueHeaderBigEndian, sizeof(valueHeaderBigEndian));
	write(this->_socketFd, &valuePayloadBigEndian, sizeof(valuePayloadBigEndian));
	this->_networkThread = std::thread([this] {
		pthread_setname_np(pthread_self(), "Network thread");
		this->_sniffANetwork();
	});
}

jetpack::Client::Program::~Program() {
	this->_isOpen = false;
	if (this->_networkThread.joinable()) this->_networkThread.join();
}
