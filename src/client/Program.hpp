//
// Created by roussierenoa on 3/28/25.
//

#ifndef PROGRAM_HPP
#define PROGRAM_HPP
#include <functional>
#include <thread>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "Graphic.hpp"
#include "Logger.hpp"

namespace Jetpack::Client {

class Program {
private:
	int _socketFd = -1;
	bool _isOpen = true;
	std::thread _networkThread;
	std::mutex _interactionMutex;

	std::function<void(UserInteractions_s)> _sendUserInteraction = ( [this] (UserInteractions_s data) {
		this->_sendPlayerInput(data);
	});

	Jetpack::Logger &_logger;
	Graphic _graphic;

	void _connnectToSocket(const char *ip, unsigned int port);
	void _handleMessageFromServer(std::string msg);
	void _sniffANetwork();
	void _sendPlayerInput(UserInteractions_s);
public:

	void loop();

	Program(const char *ip, unsigned int port, Jetpack::Logger &logger);
	~Program();
};

} // jetpack

#endif //PROGRAM_HPP
