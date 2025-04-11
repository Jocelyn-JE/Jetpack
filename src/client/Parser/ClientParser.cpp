//
// Created by roussierenoa on 3/28/25.
//
#include <arpa/inet.h>

#include <string>

#include "ClientParser.hpp"
#include "../Exception.hpp"
#include "lib.hpp"

namespace jetpack::Client {
	std::string Parser::getIp() const { return this->_ip; }

	unsigned int Parser::getPort() const { return this->_port; }

	bool Parser::isDebugMode() const { return this->_isDebugMode; }

	Parser::Parser(int ac, char **av) {
		if (ac == 2 && std::string(av[1]) == "--help") {
			throw HelpException();
		}
		if (ac < 5) {
			throw ParsingException("Not enough arguments");
		}
		if (ac > 6) {
			throw ParsingException("Too many arguments");
		}
		if (std::string(av[1]) != "-h") {
			throw ParsingException("Invalid first argument expected -h");
		}
		if (inet_addr(av[2]) == INADDR_NONE)
			throw ParsingException("Invalid IP must be a valid IPv4 address");
		if (std::string(av[3]) != "-p") {
			throw ParsingException("Invalid third argument expected -p");
		}
		if (!isNumber(av[4])) {
			throw ParsingException("Invalid Port argument");
		}
		if (ac == 6) {
			if (std::string(av[5]) == "-d")
				this->_isDebugMode = true;
			else
				throw ParsingException("Invalid sixth argument expected -d");
		}
		this->_ip = av[2];
		this->_port = std::stoi(av[4]);
	}
} // namespace jetpack::Client
