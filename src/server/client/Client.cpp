/*
** EPITECH PROJECT, 2025
** my_jetpack
** File description:
** Client
*/

#include <iostream>
#include <string>
#include "Client.hpp"

// Helper functions -----------------------------------------------------------

static bool clientDisconnect(Jetpack::server::Client const &client) {
    std::cout << "Client " << client._controlSocket.getSocketFd()
        << " disconnected" << std::endl;
    return true;
}

// Client class member functions ----------------------------------------------

Jetpack::server::Client::Client(int fd, struct sockaddr_in address,
    unsigned int id) : _controlSocket(fd, address), _id(id) {
}

Jetpack::server::Client::~Client() {
    std::cout << "Destroying client" << std::endl;
}

unsigned int Jetpack::server::Client::getId() const {
    return _id;
}

void Jetpack::server::Client::sendData(std::string data) {
    _controlSocket.writeToSocket(data);
}

bool Jetpack::server::Client::handleCommand(std::string commandLine) {
    if (commandLine == "")
        return clientDisconnect(*this);
    if (commandLine == "QUIT\r\n") {
        _controlSocket.closeSocket();
        return true;
    }
    return false;
}
