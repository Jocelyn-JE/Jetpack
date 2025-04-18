/*
** EPITECH PROJECT, 2025
** Server.hpp
** File description:
** Server
*/

#ifndef SRC_SERVER_SERVER_HPP_
#define SRC_SERVER_SERVER_HPP_

#include <memory>
#include <string>
#include <vector>

#include "Client.hpp"
#include "CommunicationHeader.hpp"
#include "Game.hpp"
#include "GameServer.hpp"
#include "PollFdList.hpp"
#include "Socket.hpp"
#include "parsing/Parser.hpp"

#define LISTEN_BACKLOG 128

namespace jetpack {
namespace server {
class Server {
 public:
    Server() = delete;
    explicit Server(Parser &parser);
    ~Server();
    static int runServer(Parser &parser);
    void setToRun() { _setToRun = true; }

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
    template <typename T>
    void sendToAllClients(T data) {
        for (size_t i = 0; i < this->_clients.size(); i++)
            this->_clients[i]->sendData(data);
    }
    template <typename T>
    void sendToAllClients(std::vector<T> data) {
        for (size_t i = 0; i < this->_clients.size(); i++)
            this->_clients[i]->sendData(data);
    }

    void sendCoinListsToPlayers();
    std::vector<uint8_t> createConnectionPacket(size_t id, size_t gameSpeed);
    std::vector<uint8_t> createStartGamePacket(void);
    std::vector<uint8_t> createPlayerListPacket(void);
    std::vector<uint8_t> createCoinListPacket(
        const std::vector<std::shared_ptr<coinsPos_t>>& coins);
    std::vector<uint8_t> createObstacleListPacket(void);
    std::vector<std::unique_ptr<Client>> _clients;
    Socket _serverSocket;
    PollFdList _socketPollList;
    unsigned int _nextClientId;
    std::shared_ptr<GameData> _gameData;
    std::shared_ptr<Game> _game;
    bool _setToRun = false;
};
}  // namespace server
}  // namespace jetpack

#endif  // SRC_SERVER_SERVER_HPP_
