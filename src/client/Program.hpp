//
// Created by roussierenoa on 3/28/25.
//

#ifndef SRC_CLIENT_PROGRAM_HPP_
#define SRC_CLIENT_PROGRAM_HPP_
#include <arpa/inet.h>
#include <sys/socket.h>

#include <functional>
#include <iostream>
#include <string>
#include <thread>

#include "Graphic.hpp"
#include "Logger.hpp"
#include "Socket.hpp"
#include "communicationHeader.hpp"

namespace jetpack::Client {
class Program {
 private:
    bool _isOpen = true;
    const char *_ip;
    unsigned int _port;
    std::thread _networkThread;
    std::mutex _communicationMutex;
    std::mutex _usernameMutex;
    std::mutex _userInteractionMutex;
    UserInteractions_s _lastUserInteraction =
        UserInteractions_s::NO_INTERACTION;
    std::string _username;
    bool _isChangeUsername = false;

    std::function<void(UserInteractions_s)> _sendUserInteraction =
        ([this](UserInteractions_s data) {
            this->_userInteractionMutex.lock();
            this->_lastUserInteraction = data;
            this->_userInteractionMutex.unlock();
        });

    std::function<void(std::string)> _changeUsername =
        ([this](std::string username) {
            this->_usernameMutex.lock();
            this->_username = username;
            this->_isChangeUsername = true;
            this->_usernameMutex.unlock();
        });

    std::function<std::string()> _getUsername = ([this]() -> std::string {
        this->_usernameMutex.lock();
        auto data = this->_username;
        this->_usernameMutex.unlock();
        return data;
    });

    jetpack::Logger &_logger;
    Graphic _graphic;
    Socket _socket;

    void _connectToSocket(const char *ip, unsigned int port);

    void _handleMessageFromServer(std::string msg);

    void _handlePayload(std::string msg, Payload_t payload, int &indexPayload);

    void _sniffANetwork();

    void _sendUpEvent();

    void _sendPlayerInput();

    void _sendNewUsername();

 public:
    void loop();

    Program(const char *ip, unsigned int port, jetpack::Logger &logger);

    ~Program();
};
}  // namespace jetpack::Client

#endif  // SRC_CLIENT_PROGRAM_HPP_
