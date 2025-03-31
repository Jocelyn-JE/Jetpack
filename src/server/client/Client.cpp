/*
** EPITECH PROJECT, 2025
** my_jetpack
** File description:
** Client
*/

#include "Client.hpp"

#include <iostream>
#include <string>

// Helper functions -----------------------------------------------------------

static bool clientDisconnect(jetpack::server::Client const &client) {
    std::cout << "Client " << client._controlSocket.getSocketFd()
              << " disconnected" << std::endl;
    return true;
}

// Client class member functions ----------------------------------------------

jetpack::server::Client::Client(int fd, struct sockaddr_in address,
                                unsigned int id)
    : _controlSocket(fd, address), _id(id) {}

jetpack::server::Client::~Client() {
    std::cout << "Destroying client" << std::endl;
}

unsigned int jetpack::server::Client::getId() const { return _id; }

void jetpack::server::Client::sendData(std::string data) {
    _controlSocket.writeToSocket(data);
}

bool jetpack::server::Client::handleCommand(std::string commandLine) {
    if (commandLine == "") {
        return clientDisconnect(*this);
    }
    if (commandLine == "QUIT\r\n") {
        _controlSocket.closeSocket();
        return true;
    }
    return false;
}
