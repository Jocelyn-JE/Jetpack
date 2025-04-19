/*
** EPITECH PROJECT, 2025
** client.cpp
** File description:
** Client
*/

#include "Client.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <CommunicationHeader.hpp>

#include "NetworksUtils.hpp"
#include "Server.hpp"

// Helper functions -----------------------------------------------------------

static bool clientDisconnect(jetpack::server::Client const &client) {
    std::cerr << "Client " << client._controlSocket.getSocketFd()
              << " disconnected" << std::endl;
    return true;
}

// Client clastd::cerr member functions
// ----------------------------------------------

jetpack::server::Client::Client(int fd, struct sockaddr_in address,
                                unsigned int id, bool debug)
    : _controlSocket(fd, address), _id(id), _debug(debug) {}

jetpack::server::Client::~Client() {
    std::cerr << "Destroying client" << std::endl;
}

unsigned int jetpack::server::Client::getId() const { return _id; }

bool jetpack::server::Client::handlePayload(std::string commandLine) {
    if (commandLine == "") {
        return clientDisconnect(*this);
    }
    if (commandLine == "QUIT\r\n") {
        return this->badInput();
    }
    return false;
}

bool jetpack::server::Client::badInput() { return false; }

bool jetpack::server::Client::handlePayload(std::vector<uint8_t> payload,
                                            std::shared_ptr<Game> game,
                                            jetpack::server::Server &server) {
    Header_t header;
    Payload_t payloadHeader;
    std::vector<uint8_t> payloadData;

    if (payload.size() == 0) return clientDisconnect(*this);
    header.rawData = (static_cast<uint16_t>(payload[0]) << 8) |
                     (static_cast<uint16_t>(payload[1]));
    if (header.magic1 != 42 || header.magic2 != 42) {
        return this->badInput();
    }
    if (_debug) {
        std::cerr << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< From ID: " << _id
                  << std::endl;
        std::cerr << "Header: magic1 = " << header.magic1
                  << ", magic2 = " << header.magic2
                  << ", nbrPayload = " << static_cast<int>(header.nbrPayload)
                  << std::endl;
    } else
        std::this_thread::sleep_for(std::chrono::microseconds(300));
    payloadData = this->_controlSocket.readFromSocket(2);
    payloadHeader.rawData = (static_cast<uint16_t>(payloadData[0]) << 8) |
                            (static_cast<uint16_t>(payloadData[1]));
    if (_debug) {
        std::cerr << "Payload: dataId = "
                  << static_cast<int>(payloadHeader.dataId) << ", dataCount = "
                  << static_cast<int>(payloadHeader.dataCount) << std::endl;
    } else
        std::this_thread::sleep_for(std::chrono::microseconds(300));
    if (payloadHeader.dataId >= INVALID) return this->badInput();
    if (payloadHeader.dataId == PLAYER_INPUT)
        return this->handleInput(payloadData, game);
    if (payloadHeader.dataId == START)
        return this->handleStart(payloadData, server);
    return this->badInput();
}

bool jetpack::server::Client::handleInput(std::vector<uint8_t> payloadData,
                                          std::shared_ptr<Game> game) {
    std::vector<uint8_t> payloadData2;

    payloadData2 =
        this->_controlSocket.readFromSocket(getPayloadSize(PLAYER_INPUT));
    if (_debug) {
        std::cerr << "Handling input from player: ";
        for (const auto &byte : payloadData2) {
            std::cerr << std::hex << std::uppercase << std::setw(2)
                      << std::setfill('0') << static_cast<int>(byte) << " ";
            std::cerr << std::dec;
        }
    } else
        std::this_thread::sleep_for(std::chrono::microseconds(800));
    std::cerr << std::dec << std::endl;
    bool jetpackOn = static_cast<bool>(payloadData2[0]);
    if (jetpackOn) {
        game->getPlayer(_id)->is_jetpack_on = true;
    } else {
        game->getPlayer(_id)->is_jetpack_on = false;
    }
    (void)payloadData;
    return false;
}

bool jetpack::server::Client::handleStart(std::vector<uint8_t> payloadData,
                                          jetpack::server::Server &server) {
    std::cerr << "Handling start command from client ID: " << _id << std::endl;

    uint8_t startFlag = payloadData[0];
    std::cerr << "Start flag: " << static_cast<int>(startFlag) << std::endl;

    server.setToRun();

    return false;
}
