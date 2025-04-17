/*
** EPITECH PROJECT, 2025
** my_jetpack
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

// Helper functions -----------------------------------------------------------

static bool clientDisconnect(jetpack::server::Client const &client) {
    std::cerr << "Client " << client._controlSocket.getSocketFd()
              << " disconnected" << std::endl;
    return true;
}

// Client clastd::cerr member functions
// ----------------------------------------------

jetpack::server::Client::Client(int fd, struct sockaddr_in address,
                                unsigned int id)
    : _controlSocket(fd, address), _id(id) {}

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

bool jetpack::server::Client::badInput() {
    std::cerr << "Client sent garbage data" << std::endl;
    return false;
}

bool jetpack::server::Client::handlePayload(std::vector<uint8_t> payload,
                                            std::shared_ptr<Game> game) {
    for (const auto &byte : payload) {
        std::cerr << std::hex << std::uppercase << std::setw(2)
                  << std::setfill('0') << static_cast<int>(byte) << " ";
        std::cerr << std::dec;
    }
    Header_t header;
    Payload_t payloadHeader;
    std::vector<uint8_t> payloadData;

    if (payload.size() == 0) return clientDisconnect(*this);
    header.rawData = (static_cast<uint16_t>(payload[0]) << 8) |
                     (static_cast<uint16_t>(payload[1]));
    if (header.magic1 != 42 || header.magic2 != 42) {
        return this->badInput();
    }
    std::cerr << "Header: magic1 = " << header.magic1
              << ", magic2 = " << header.magic2
              << ", nbrPayload = " << static_cast<int>(header.nbrPayload)
              << std::endl;
    payloadData = this->_controlSocket.readFromSocket(2);
    payloadHeader.rawData = (static_cast<uint16_t>(payloadData[0]) << 8) |
                            (static_cast<uint16_t>(payloadData[1]));
    if (payloadHeader.dataId >= INVALID) return this->badInput();
    if (payloadHeader.dataId == PLAYER_INPUT)
        return this->handleInput(payloadData, game);
    if (payloadHeader.dataId == START)
        return this->handleInput(payloadData, game);
    return this->badInput();
}

bool jetpack::server::Client::handleInput(std::vector<uint8_t> payloadData,
                                          std::shared_ptr<Game> game) {
    std::vector<uint8_t> payloadData2;

    payloadData2 =
        this->_controlSocket.readFromSocket(getPayloadSize(PLAYER_INPUT));

    std::cerr << "Handling input from player: ";
    for (const auto &byte : payloadData2) {
        std::cerr << std::hex << std::uppercase << std::setw(2)
                  << std::setfill('0') << static_cast<int>(byte) << " ";
        std::cerr << std::dec;
    }
    std::cerr << std::dec << std::endl;
    bool jetpackOn = static_cast<bool>(payloadData2[0]);
    std::cerr << "Jetpack on: " << jetpackOn << std::endl;
    if (jetpackOn) {
        game->getPlayer(_id)->is_jetpack_on = true;
    } else {
        game->getPlayer(_id)->is_jetpack_on = false;
    }
    return false;
}
