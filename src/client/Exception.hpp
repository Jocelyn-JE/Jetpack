//
// Created by roussierenoa on 3/25/25.
//

#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <exception>

class NetworkException : public std::exception {
private:
	std::string _msg;

public:
	const char *what() const noexcept override {return this->_msg.c_str();};

	NetworkException(const std::string &msg) : _msg(msg) {};
	~NetworkException() override = default;
};

class ParsingException : public std::exception {
private:
	std::string _msg;

public:
	const char *what() const noexcept override {return this->_msg.c_str();};

	ParsingException(const std::string &msg) : _msg(msg) {};
	~ParsingException() override = default;
};

#endif //EXCEPTION_HPP
