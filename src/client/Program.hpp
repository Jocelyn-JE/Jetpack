//
// Created by roussierenoa on 3/28/25.
//

#ifndef SRC_CLIENT_PROGRAM_HPP_
#define SRC_CLIENT_PROGRAM_HPP_
#include <arpa/inet.h>
#include <sys/socket.h>

#include <functional>
#include <iostream>
#include <string>
#include <thread>

#include "Graphic.hpp"
#include "Logger.hpp"

namespace jetpack::Client {
	class Program {
	private:
		int _socketFd = -1;
		bool _isOpen = true;
		std::thread _networkThread;
		std::mutex _interactionMutex;

		std::function<void(UserInteractions_s)> _sendUserInteraction =
				([this](UserInteractions_s data) { this->_sendPlayerInput(data); });

		jetpack::Logger &_logger;
		Graphic _graphic;

		void _connnectToSocket(const char *ip, unsigned int port);

		void _handleMessageFromServer(std::string msg);

		void _sniffANetwork();

		void _sendPlayerInput(UserInteractions_s);

	public:
		void loop();

		Program(const char *ip, unsigned int port, jetpack::Logger &logger);

		~Program();
	};
} // namespace jetpack::Client

#endif  // SRC_CLIENT_PROGRAM_HPP_
