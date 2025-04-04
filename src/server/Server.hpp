/*
** EPITECH PROJECT, 2025
** my_jetpack
** File description:
** Server
*/

#ifndef SRC_SERVER_SERVER_HPP_
#define SRC_SERVER_SERVER_HPP_

#include <memory>
#include <string>
#include <vector>

#include "Client.hpp"
#include "PollFdList.hpp"
#include "Socket.hpp"
#define LISTEN_BACKLOG 128

namespace jetpack {
namespace server {
class Server {
 public:
    Server() = delete;
    explicit Server(int port);
    ~Server();
    static int runServer(int port);

 private:
    int pollSockets();
    // Iterate through all clients with _clients.size() (+ 1 for the server
    // socket) and update/execute depending on values read from the sockets.
    // Server socket is _serverSocket polling is _socketPollList[0]
    void updateSockets();
    void handleConnection();
    // This function does not close the given socket, it only removes it from
    // the socket list and poll list
    void handleDisconnection(int socketIndex);
    bool isClosed();
    void sendToAllClients(std::string data);
    std::vector<std::unique_ptr<Client>> _clients;
    Socket _serverSocket;
    PollFdList _socketPollList;
    unsigned int _nextClientId;
};
}  // namespace server
}  // namespace jetpack

#endif  // SRC_SERVER_SERVER_HPP_
