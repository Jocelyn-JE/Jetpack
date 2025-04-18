/*
** EPITECH PROJECT, 2025
** Client.hpp
** File description:
** Client
*/

#ifndef SRC_SERVER_CLIENT_CLIENT_HPP_
#define SRC_SERVER_CLIENT_CLIENT_HPP_

#include <bits/stdc++.h>

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Game.hpp"
#include "GameServer.hpp"
#include "Socket.hpp"

namespace jetpack::server {
class Server;
}

namespace jetpack {
namespace server {
class Client {
 public:
    Client() = delete;
    Client(int fd, struct sockaddr_in address, unsigned int id);
    ~Client();
    Socket _controlSocket;
    // handlePayload returns true if the client should be disconnected
    bool handlePayload(std::string commandLine);
    // handlePayload returns true if the client should be disconnected
    bool handlePayload(std::vector<uint8_t> payload, std::shared_ptr<Game> game,
                       jetpack::server::Server &server);
    template <typename T>
    bool handlePayload(std::vector<T> payload) {
        if (payload.size() == 0) return true;
        return false;
    }
    template <typename T>
    void sendData(T data) {
        _controlSocket.writeToSocket(data);
    }
    template <typename T>
    void sendData(std::vector<T> data) {
        _controlSocket.writeToSocket(data);
    }
    unsigned int getId() const;
    bool handleInput(std::vector<uint8_t> payloadData,
                     std::shared_ptr<Game> game);
    bool handleStart(std::vector<uint8_t> payloadData,
                     jetpack::server::Server &server);

 private:
    bool badInput();
    unsigned int _id;
};
}  // namespace server
}  // namespace jetpack

#endif  // SRC_SERVER_CLIENT_CLIENT_HPP_
