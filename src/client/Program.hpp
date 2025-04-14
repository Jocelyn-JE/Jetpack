//
// Created by roussierenoa on 3/28/25.
//

#ifndef SRC_CLIENT_PROGRAM_HPP_
#define SRC_CLIENT_PROGRAM_HPP_
#include <arpa/inet.h>
#include <sys/socket.h>

#include <vector>
#include <functional>
#include <iostream>
#include <string>
#include <thread>
#include "Graphic.hpp"
#include "Logger.hpp"
#include "Socket.hpp"
#include "communicationHeader.hpp"
#include "Auth/Auth.hpp"

namespace jetpack::Client {
class Program {
 private:
    bool _isOpen = true;
    std::string _ip;
    unsigned int _port;
    bool _manualReco = false;
    std::thread _networkThread;
    std::mutex _portIpMutex;
    std::mutex _communicationMutex;
    std::mutex _usernameMutex;
    std::mutex _userInteractionMutex;
    UserInteractions_s _lastUserInteraction =
        UserInteractions_s::NO_INTERACTION;
    bool _isChangeUsername = false;

    Auth _auth;
    jetpack::Logger &_logger;
    Graphic _graphic;
    Socket _socket;

    std::function<void(UserInteractions_s)> _sendUserInteraction =
        ([this](UserInteractions_s data) {
            this->_userInteractionMutex.lock();
            this->_lastUserInteraction = data;
            this->_userInteractionMutex.unlock();
    });

    std::function<void(std::string)> _changeUsername =
        ([this](std::string username) {
            this->_usernameMutex.lock();
            this->_auth.setUsername(username);
            this->_isChangeUsername = true;
            this->_usernameMutex.unlock();
    });

    std::function<std::string()> _getUsername = ([this]() -> std::string {
        this->_usernameMutex.lock();
        auto data = this->_auth.getUsername();
        this->_usernameMutex.unlock();
        return data;
    });

    std::function<std::pair<std::string, std::string>()> _getSocketSettings = ([this]() -> std::pair<std::string, std::string> {
        this->_portIpMutex.lock();
        auto data = std::make_pair(this->_ip, std::to_string(this->_port));
        this->_portIpMutex.unlock();
        return data;
    });

    std::function<void(std::pair<std::string, int>)> _setSocketSettings = 
        ([this](std::pair<std::string, int> data) {
            this->_portIpMutex.lock();
            this->_manualReco = true;
            this->_ip = data.first;
            this->_port = data.second;
            this->_portIpMutex.unlock();
    });

    void _setSize_tData(std::vector<unsigned char> msg);

    void _getServerMessage();

    void _connectToSocket(const char *ip, unsigned int port);

    void _handleMessageFromServer(Payload_t payload);

    void _handlePayload(std::vector<unsigned char> msg, Payload_t payload);

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
