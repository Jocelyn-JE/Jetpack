/*
** EPITECH PROJECT, 2025
** my_jetpack
** File description:
** Client
*/

#ifndef SERVER_SRC_CLIENT_CLIENT_HPP_
    #define SERVER_SRC_CLIENT_CLIENT_HPP_

    #include <bits/stdc++.h>
    #include <unordered_map>
    #include <memory>
    #include <string>
    #include "./server/src/sockets/Socket.hpp"

namespace jetpack {
namespace server {
class Client {
 public:
    Client() = delete;
    Client(int fd, struct sockaddr_in address, unsigned int id);
    ~Client();
    Socket _controlSocket;
    // handleCommand returns true if the client should be disconnected
    bool handleCommand(std::string commandLine);
    void sendData(std::string data);
    unsigned int getId() const;
 private:
    unsigned int _id;
};
}  // namespace server
}  // namespace jetpack

#endif  // SERVER_SRC_CLIENT_CLIENT_HPP_
