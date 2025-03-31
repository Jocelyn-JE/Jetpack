/*
** EPITECH PROJECT, 2025
** my_jetpack
** File description:
** Socket
*/

#include "Socket.hpp"
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

Socket::Socket(int fd, struct sockaddr_in address) noexcept
    : _closeSocketOnDestruction(false) {
    _socketFd = fd;
    _address = address;
    std::cout << "Fetched new socket on fd: " << _socketFd << std::endl;
}

Socket::Socket(int domain, int type, int protocol) noexcept(false)
    : _closeSocketOnDestruction(true) {
    _socketFd = socket(domain, type, protocol);
    if (_socketFd == -1) {
        throw Socket::SocketError("Socket creation failed: " +
                                  std::string(strerror(errno)));
    }
    std::cout << "Created socket on fd: " << _socketFd << std::endl;
}

Socket::~Socket() noexcept(false) {
    if (_socketFd >= 0 && _closeSocketOnDestruction) {
        if (close(_socketFd) == -1) {
            throw Socket::SocketError("Failed to close socket: " +
                                      std::string(strerror(errno)));
        }
        std::cout << "Closed socket on fd: " << _socketFd << std::endl;
    }
    std::cout << "Removed socket on fd " << _socketFd << " from server"
              << std::endl;
}

bool Socket::closesOnDestroy() const noexcept {
    return _closeSocketOnDestruction;
}

void Socket::closeSocket() noexcept(false) {
    if (close(_socketFd) == -1) {
        throw Socket::SocketError("Failed to close socket: " +
                                  std::string(strerror(errno)));
    }
    std::cout << "Closed socket on fd: " << _socketFd << std::endl;
    _closeSocketOnDestruction = false;
}

int Socket::getSocketFd() const noexcept { return _socketFd; }

void Socket::bindSocket(uint16_t port) noexcept(false) {
    memset(&_address, 0, sizeof(_address));
    _address.sin_family = AF_INET;
    _address.sin_port = htons(port);
    _address.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(_socketFd, (const struct sockaddr *)&_address, sizeof(_address)) ==
        -1) {
        throw Socket::SocketError("Bind failed: " +
                                  std::string(strerror(errno)));
    }
    socklen_t len = sizeof(_address);
    if (getsockname(_socketFd, (struct sockaddr *)&_address, &len) == -1) {
        throw Socket::SocketError("Getsockname failed: " +
                                  std::string(strerror(errno)));
    }
}

void Socket::listenSocket(int backlog) noexcept(false) {
    if (listen(_socketFd, backlog)) {
        throw Socket::SocketError("Listen failed: " +
                                  std::string(strerror(errno)));
    }
}

void Socket::connectSocket(const struct sockaddr *address,
                           socklen_t addressLen) noexcept(false) {
    if (connect(_socketFd, address, addressLen) == -1) {
        throw Socket::SocketError("Connect failed: " +
                                  std::string(strerror(errno)));
    }
}

void Socket::connectSocket(const char *ip_address,
                           uint16_t port) noexcept(false) {
    struct sockaddr_in address;

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(ip_address);
    if (connect(_socketFd, (const struct sockaddr *)&address,
                sizeof(address)) == -1) {
        throw Socket::SocketError("Connect failed: " +
                                  std::string(strerror(errno)));
    }
}

Socket::SocketError::SocketError(std::string message) noexcept {
    _message = message;
}

Socket::SocketError::~SocketError() noexcept {}

const char *Socket::SocketError::what() const noexcept {
    return _message.c_str();
}

void Socket::writeToSocket(std::string str) noexcept(false) {
    if (write(this->_socketFd, str.c_str(), str.length()) == -1) {
        throw Socket::SocketError("Write on fd " + _socketFd +
                                  (" failed: " + std::string(strerror(errno))));
    }
}

void Socket::writeToSocket(std::vector<uint8_t> buffer) noexcept(false) {
    if (write(this->_socketFd, buffer.data(), buffer.size()) == -1) {
        throw Socket::SocketError("Write on fd " + _socketFd +
                                  (" failed: " + std::string(strerror(errno))));
    }
}

std::string Socket::readFromSocket() noexcept(false) {
    char buffer[BUFSIZ];
    int bytes_read = read(_socketFd, buffer, sizeof(buffer));

    buffer[bytes_read] = '\0';
    if (bytes_read < 0) {
        throw Socket::SocketError("Read on fd " + _socketFd +
                                  (" failed: " + std::string(strerror(errno))));
    }
    return buffer;
}

std::vector<uint8_t> Socket::readFromSocket(size_t size) noexcept(false) {
    std::vector<uint8_t> buffer(size);
    int bytes_read = read(_socketFd, buffer.data(), buffer.size());

    if (bytes_read < 0) {
        throw Socket::SocketError("Read on fd " + _socketFd +
                                  (" failed: " + std::string(strerror(errno))));
    }
    return buffer;
}

struct sockaddr_in Socket::getAddress() const noexcept { return _address; }

void Socket::setCloseOnDestroy(bool closeOnDestroy) noexcept {
    _closeSocketOnDestruction = closeOnDestroy;
}
