/*
** EPITECH PROJECT, 2025
** my_jetpack
** File description:
** Parser
*/

#include <bits/algorithmfwd.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include "Parser.hpp"

Jetpack::Parser::Parser(int argc, char **argv) :
    _args(argv, argv + argc), _argc(argc) {
}

Jetpack::Parser::~Parser() {
}

static bool isNumber(const std::string &str) {
    if (str.empty())
        return false;
    for (char c : str) {
        if (!isdigit(c))
            return false;
    }
    return true;
}

std::string Jetpack::Parser::getServerUsage() {
    return R"(USAGE: ./jetpack_server port
    port is the port number on which the server socket listens)";
}

void Jetpack::Parser::parseServerArgs() {
    if (this->_argc == 2 && this->_args[1] == "-help") {
        std::cout << getServerUsage() << std::endl;
        exit(0);
    }
    if (this->_argc < 2)
        throw ParsingError("Not enough args");
    if (this->_argc > 2)
        throw ParsingError("Too many args");
    if (!isNumber(this->_args[1]))
        throw ParsingError("Port must be a number");
}

Jetpack::Parser::ParsingError::ParsingError(std::string message) {
    this->_message = message;
}

Jetpack::Parser::ParsingError::~ParsingError() {
}

const char *Jetpack::Parser::ParsingError::what() const noexcept {
    return this->_message.c_str();
}
