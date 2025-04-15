/*
** EPITECH PROJECT, 2025
** my_jetpack
** File description:
** Client
*/

#include "Client.hpp"

#include <iostream>
#include <string>
#include <vector>

#include <CommunicationHeader.hpp>

// Helper functions -----------------------------------------------------------

static bool clientDisconnect(jetpack::server::Client const &client) {
    std::cerr << "Client " << client._controlSocket.getSocketFd()
              << " disconnected" << std::endl;
    return true;
}

// Client class member functions ----------------------------------------------

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
        return this->closeAndDisconnect();
    }
    return false;
}

bool jetpack::server::Client::closeAndDisconnect() {
    _controlSocket.closeSocket();
    return true;
}

bool jetpack::server::Client::handlePayload(std::vector<uint8_t> payload) {
    Header_t header;

    if (payload.size() == 0) return clientDisconnect(*this);
    header.rawData = (static_cast<uint16_t>(payload[0]) << 8) |
                     (static_cast<uint16_t>(payload[1]));
    if (header.magic1 != 42 || header.magic2 != 42)
        return this->closeAndDisconnect();
    for (int i = 0; i < header.nbrPayload; i++) {
        std::vector<uint8_t> payloadData =
            this->_controlSocket.readFromSocket(2);
        Payload_t payloadHeader;
        payloadHeader.rawData = (static_cast<uint16_t>(payloadData[0]) << 8) |
                                (static_cast<uint16_t>(payloadData[1]));
        if (payloadHeader.dataId >= INVALID) return this->closeAndDisconnect();
    }
    return false;
}
