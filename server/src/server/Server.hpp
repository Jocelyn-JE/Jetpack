/*
** EPITECH PROJECT, 2025
** my_jetpack
** File description:
** Server
*/

#ifndef SERVER_SRC_SERVER_SERVER_HPP_
    #define SERVER_SRC_SERVER_SERVER_HPP_

    #include <memory>
    #include <vector>
    #include <string>
    #include "./server/src/sockets/Socket.hpp"
    #include "./server/src/server/PollFdList.hpp"
    #include "./server/src/client/Client.hpp"
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
    void updateSockets();
    void handleConnection();
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

#endif  // SERVER_SRC_SERVER_SERVER_HPP_
