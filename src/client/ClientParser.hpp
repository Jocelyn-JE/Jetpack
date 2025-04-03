//
// Created by roussierenoa on 3/28/25.
//

#ifndef SRC_CLIENT_CLIENTPARSER_HPP_
#define SRC_CLIENT_CLIENTPARSER_HPP_

#include <iostream>
#include <string>

namespace jetpack::Client {
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
} // namespace jetpack::Client

#endif  // SRC_CLIENT_CLIENTPARSER_HPP_
