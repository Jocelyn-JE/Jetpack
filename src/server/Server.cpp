/*
** EPITECH PROJECT, 2025
** my_jetpack
** File description:
** Server
*/

#include "Server.hpp"

#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>

#include <iostream>
#include <string>

#include "Socket.hpp"

volatile sig_atomic_t stopFlag = 0;

static void handler(int signum) { stopFlag = signum; }

int Jetpack::server::Server::runServer(int port) {
    int poll_result = 0;
    struct sigaction sa;

    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
    try {
        Jetpack::server::Server server(port);

        while (poll_result != -1 && !stopFlag) {
            poll_result = server.pollSockets();
            server.updateSockets();
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return 0;
}

//-----------------------------------------------------------------------------

Jetpack::server::Server::Server(int port)
    : _serverSocket(AF_INET, SOCK_STREAM, 0),
      _socketPollList(_serverSocket.getSocketFd()),
      _nextClientId(0) {
    _serverSocket.bindSocket(port);
    _serverSocket.listenSocket(LISTEN_BACKLOG);
    std::cout << "Server started on port " << port << std::endl;
}

Jetpack::server::Server::~Server() {
}

int Jetpack::server::Server::pollSockets() {
    int result = poll(_socketPollList.data(), _clients.size() + 1, -1);
    if (result == -1) {
        throw Socket::SocketError("Poll failed: " +
                                  std::string(strerror(errno)));
    }
    return result;
}

bool Jetpack::server::Server::isClosed() {
    return !_serverSocket.closesOnDestroy();
}

// Iterate through all clients with _clients.size() (+ 1 for the server socket)
// and update/execute depending on values read from the sockets.
// Server socket is _serverSocket polling is _socketPollList[0]
void Jetpack::server::Server::updateSockets() {
    std::string socketStr;
    std::string buffer;

    for (std::size_t i = 0; i < _socketPollList.size(); i++) {
        if (_socketPollList[i].revents & POLLIN && i == 0) {
            std::string playerList = "player_list:";
            handleConnection();
            for (size_t i = 0; i < _clients.size(); i++) {
                playerList += std::to_string(_clients[i]->getId());
                if (i < _clients.size() - 1) {
                    playerList += ",";
                }
            }
            sendToAllClients(playerList);
        }
        if (_socketPollList[i].revents & POLLIN && i != 0) {
            buffer = _clients[i - 1]->_controlSocket.readFromSocket();
            if (_clients[i - 1]->handleCommand(buffer)) {
                sendToAllClients("disconnected:" +
                                 std::to_string(_clients[i - 1]->getId()));
                handleDisconnection(i);
            } else {
                sendToAllClients(std::to_string(_clients[i - 1]->getId()) +
                                 ":" + buffer);
            }
        }
    }
}

// This function does not close the given socket, it only removes it from the
// socket list and poll list
void Jetpack::server::Server::handleDisconnection(int socketIndex) {
    _clients.erase(_clients.begin() + (socketIndex - 1));
    _socketPollList.removeSocket(_socketPollList[socketIndex].fd);
}

void Jetpack::server::Server::handleConnection() {
    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    int client_socket =
        accept(_serverSocket.getSocketFd(), (struct sockaddr *)&client_addr,
               &client_addr_size);

    this->_clients.push_back(std::make_unique<Client>(
        client_socket, client_addr, this->_nextClientId));
    this->_nextClientId++;
    this->_socketPollList.addSocket(client_socket, POLLIN);
    std::cout << inet_ntoa(client_addr.sin_addr) << ":"
              << ntohs(client_addr.sin_port)
              << " connected fd: " << client_socket << std::endl;
    this->_clients.back()->_controlSocket.writeToSocket(
        "id:" + std::to_string(this->_clients.back()->getId()));
}

void Jetpack::server::Server::sendToAllClients(std::string data) {
    for (size_t i = 0; i < this->_clients.size(); i++) {
        this->_clients[i]->sendData(data);
    }
}
