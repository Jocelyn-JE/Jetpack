/*
** EPITECH PROJECT, 2025
** my_jetpack
** File description:
** Parser
*/

#include <arpa/inet.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <ranges>
#include <string>
#include <vector>

#include "Parser.hpp"

jetpack::Parser::Parser(int argc, char** argv)
    : _args(argv, argv + argc), _argc(argc) {}

jetpack::Parser::~Parser() {}

static bool isNumber(const std::string& str) {
    if (str.empty()) {
        return false;
    }
    for (char c : str) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

std::string jetpack::Parser::getServerUsage() {
    return R"(USAGE: ./jetpack_server [-d] [-m map_file] [-p port]
    -d: enable debug mode
    -m map_file: specify the map file (default: map.joy)
    -p port: specify the port number (default: 4242))";
}

std::string jetpack::Parser::getClientUsage() {
    return R"(USAGE: ./jetpack_client ip port
    ip is the ip address of the server
    port is the port number on which the client socket connects)";
}

void jetpack::Parser::parseServerArgs() {
    if (this->_argc == 2 && this->_args[1] == "-help") {
        std::cerr << getServerUsage() << std::endl;
        exit(0);
    }
    if (this->_argc < 2) {
        throw ParsingError("Not enough args");
    }
    if (this->_argc > 2) {
        throw ParsingError("Too many args");
    }
    if (!isNumber(this->_args[1])) {
        throw ParsingError("Port must be a number");
    }
}

void jetpack::Parser::parseClientArgs() {
    if (this->_argc == 2 && this->_args[1] == "-help") {
        std::cerr << getClientUsage() << std::endl;
        exit(0);
    }
    if (this->_argc < 3) {
        throw ParsingError("Not enough args");
    }
    if (this->_argc > 3) {
        throw ParsingError("Too many args");
    }
    if (!isNumber(this->_args[2])) {
        throw ParsingError("Port must be a number");
    }
    if (inet_addr(this->_args[1].c_str()) == INADDR_NONE) {
        throw ParsingError("IP must be a valid IPv4 address");
    }
}

size_t jetpack::Parser::findFlagIndex(const std::string& flag) const {
    auto it = std::find(_args.begin(), _args.end(), flag);
    return it - _args.begin();
}

void jetpack::Parser::parseServerFlags(GameData& data) {
    if (this->_argc == 2 && this->_args[1] == "-help") {
        std::cerr << getServerUsage() << std::endl;
        exit(0);
    }

    size_t idx;

    // Parse debug flag
    idx = findFlagIndex("-d");
    if (idx < _args.size()) {
        data.debug = true;
    }

    // Parse map flag
    idx = findFlagIndex("-m");
    if (idx < _args.size() - 1) {
        data.filename = _args[idx + 1];
    } else if (idx < _args.size()) {
        throw ParsingError("Missing filename after -m flag");
    }

    // Parse port flag
    idx = findFlagIndex("-p");
    if (idx < _args.size() - 1) {
        if (!isNumber(_args[idx + 1])) {
            throw ParsingError("Port must be a number");
        }
        data.port = std::stoi(_args[idx + 1]);
    } else if (idx < _args.size()) {
        throw ParsingError("Missing port number after -p flag");
    }

    /// FAKE PLAYER
    // data.players[0] = std::make_shared<gameplayer_t>(0, "Player1");
    // data.players[1] = std::make_shared<gameplayer_t>(1, "Player2");
    // data.players[0]->is_jetpack_on = true;
    // data.players[1]->is_jetpack_on = true;
}

jetpack::Parser::ParsingError::ParsingError(std::string message) {
    this->_message = message;
}

jetpack::Parser::ParsingError::~ParsingError() {}

const char* jetpack::Parser::ParsingError::what() const noexcept {
    return this->_message.c_str();
}

void jetpack::MapParser::processLine(
    const std::string& line, int y,
    std::vector<std::shared_ptr<coinsPos_t>>& coins,
    std::vector<std::shared_ptr<obstacle_t>>& obstacles) {
    for (size_t x = 0; x < line.length(); x++) {
        if (line[x] == '#') {
            obstacles.push_back(std::make_shared<obstacle_t>(x, y));
        } else if (line[x] == '*') {
            coins.push_back(std::make_shared<coinsPos_t>(x, y));
        }
    }
}

bool jetpack::MapParser::parseMap(
    const std::string& filename,
    std::vector<std::shared_ptr<coinsPos_t>>& coins,
    std::vector<std::shared_ptr<obstacle_t>>& obstacles) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open map file: " << filename << std::endl;
        return false;
    }

    coins.clear();
    obstacles.clear();

    std::string line;
    int y = 0;
    while (std::getline(file, line)) {
        processLine(line, y, coins, obstacles);
        y++;
    }
    return true;
}
