/*
** EPITECH PROJECT, 2025
** my_jetpack
** File description:
** Parser
*/

#include <iostream>
#include <string>
#include "./server/src/parsing/Parser.hpp"

jetpack::Parser::Parser(int argc, char **argv) : _args(argv, argv + argc),
    _argc(argc) {
}

jetpack::Parser::~Parser() {
}

static bool isNumber(std::string str) {
    char *p = 0;

    strtol(str.c_str(), &p, 10);
    return *p == 0;
}

std::string jetpack::Parser::getUsage() {
    return R"(USAGE: ./jetpack_server port
    port is the port number on which the server socket listens)";
}

void jetpack::Parser::parseArgs() {
    if (_argc == 2 && _args[1] == "-help") {
        std::cout << getUsage() << std::endl;
        exit(0);
    }
    if (_argc < 2)
        throw jetpack::Parser::ParsingError("Not enough args");
    if (_argc > 2)
        throw jetpack::Parser::ParsingError("Too many args");
    if (!isNumber(_args[1]))
        throw jetpack::Parser::ParsingError("Port must be a number");
}

jetpack::Parser::ParsingError::ParsingError(std::string message) {
    _message = message;
}

jetpack::Parser::ParsingError::~ParsingError() {
}

const char *jetpack::Parser::ParsingError::what() const noexcept {
    return _message.c_str();
}
