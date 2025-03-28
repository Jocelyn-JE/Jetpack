//
// Created by roussierenoa on 3/28/25.
//

#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <iostream>

namespace Jetpack::Client {

	class Parser {
	private:
		std::string _ip = "127.0.0.1";
		unsigned int _port = 0;
		bool _isDebugMode = false;
	public:
		std::string getIp() const;
		unsigned int getPort() const;
		bool isDebugMode() const;
		Parser() = delete;
		Parser(int ac, char **av);
	};

}

#endif //PARSER_HPP
