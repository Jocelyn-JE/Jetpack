//
// Created by roussierenoa on 3/28/25.
//

#ifndef SRC_CLIENT_PROGRAM_HPP_
#define SRC_CLIENT_PROGRAM_HPP_
#include <communicationHeader.hpp>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <functional>
#include <iostream>
#include <string>
#include <thread>

#include <Socket.hpp>

#include "Graphic.hpp"
#include "Logger.hpp"

namespace jetpack::Client {
	class Program {
	private:
		bool _isOpen = true;
		const char *_ip;
		unsigned int _port;
		std::thread _networkThread;
		std::mutex _interactionMutex;
		UserInteractions_s _lastUserInteraction = UserInteractions_s::NO_INTERACTION;

		std::function<void(UserInteractions_s)> _sendUserInteraction =
			([this](UserInteractions_s data) {
				this->_interactionMutex.lock();
				this->_lastUserInteraction = data;
				this->_interactionMutex.unlock();
			});

		std::function<void()> _sendChangeUserName =
		([this]() { this->_sendChangeUsername(); });

		jetpack::Logger &_logger;
		Graphic _graphic;
		Socket _socket;

		void _connnectToSocket(const char *ip, unsigned int port);

		void _handleMessageFromServer(std::string msg);

		void _handlePayload(std::string msg, Payload_t payload, int &indexPayload);

		void _sniffANetwork();

		void _sendPlayerInput();

		void _sendChangeUsername();

	public:
		void loop();

		Program(const char *ip, unsigned int port, jetpack::Logger &logger);

		~Program();
	};
} // namespace jetpack::Client

#endif  // SRC_CLIENT_PROGRAM_HPP_
