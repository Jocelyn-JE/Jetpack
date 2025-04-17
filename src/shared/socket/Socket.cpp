/*
** EPITECH PROJECT, 2025
** my_jetpack
** File description:
** Socket
*/

#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

#include "Socket.hpp"

Socket::Socket(int fd, struct sockaddr_in address) noexcept
    : _closeSocketOnDestruction(false) {
    this->_socketFd = fd;
    this->_address = address;
    std::cerr << "Fetched new socket on fd: " << this->_socketFd << std::endl;
}

Socket::Socket(int domain, int type, int protocol) noexcept(false)
    : _closeSocketOnDestruction(true) {
    this->_socketFd = socket(domain, type, protocol);
    if (this->_socketFd == -1) {
        throw Socket::SocketError("Socket creation failed: " +
                                  std::string(strerror(errno)));
    }
    std::cerr << "Created socket on fd: " << this->_socketFd << std::endl;
}

Socket::~Socket() noexcept(false) {
    if (this->_socketFd >= 0 && this->_closeSocketOnDestruction) {
        if (close(this->_socketFd) == -1) {
            throw Socket::SocketError("Failed to close socket: " +
                                      std::string(strerror(errno)));
        }
        std::cerr << "Closed socket on fd: " << this->_socketFd << std::endl;
    }
    std::cerr << "Removed socket on fd " << this->_socketFd << " from server"
              << std::endl;
}

void Socket::resetSocket(int domain, int type, int protocol) {
    if (this->_socketFd != -1) return;
    this->_socketFd = socket(domain, type, protocol);
    if (this->_socketFd == -1) {
        throw Socket::SocketError("Socket creation failed: " +
                                  std::string(strerror(errno)));
    }
    std::cerr << "Created socket on fd: " << this->_socketFd << std::endl;
}

bool Socket::closesOnDestroy() const noexcept {
    return this->_closeSocketOnDestruction;
}

void Socket::closeSocket() noexcept(false) {
    if (this->_socketFd == -1) {
        std::cerr << "Socket already closed on fd: " << this->_socketFd
                  << std::endl;
        return;
    }
    if (close(this->_socketFd) == -1) {
        throw Socket::SocketError("Failed to close socket: " +
                                  std::string(strerror(errno)));
    }
    std::cerr << "Closed socket on fd: " << this->_socketFd << std::endl;
    this->_socketFd = -1;
    this->_closeSocketOnDestruction = false;
}

int Socket::getSocketFd() const noexcept { return _socketFd; }

void Socket::bindSocket(uint16_t port) noexcept(false) {
    memset(&this->_address, 0, sizeof(_address));
    this->_address.sin_family = AF_INET;
    this->_address.sin_port = htons(port);
    this->_address.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(this->_socketFd, (const struct sockaddr *)&this->_address,
             sizeof(this->_address)) == -1) {
        throw Socket::SocketError("Bind failed: " +
                                  std::string(strerror(errno)));
    }
    socklen_t len = sizeof(this->_address);
    if (getsockname(this->_socketFd, (struct sockaddr *)&this->_address,
                    &len) == -1) {
        throw Socket::SocketError("Getsockname failed: " +
                                  std::string(strerror(errno)));
    }
}

void Socket::listenSocket(int backlog) noexcept(false) {
    if (listen(this->_socketFd, backlog)) {
        throw Socket::SocketError("Listen failed: " +
                                  std::string(strerror(errno)));
    }
}

void Socket::connectSocket(const struct sockaddr *address,
                           socklen_t addressLen) noexcept(false) {
    if (connect(this->_socketFd, address, addressLen) == -1) {
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
    int connect_result = connect(
        this->_socketFd, (const struct sockaddr *)&address, sizeof(address));
    if (connect_result == -1) {
        throw Socket::SocketError(
            "Connect failed to " + std::string(ip_address) + ":" +
            std::to_string(port) + " - " + std::string(strerror(errno)));
    }
    std::cerr << "Successfully connected to " << ip_address << ":" << port
              << " on fd: " << this->_socketFd << std::endl;
}

Socket::SocketError::SocketError(std::string message) noexcept {
    this->_message = message;
}

Socket::SocketError::~SocketError() noexcept {}

const char *Socket::SocketError::what() const noexcept {
    return this->_message.c_str();
}

void Socket::writeToSocket(std::string str) noexcept(false) {
    if (write(this->_socketFd, str.c_str(), str.length()) == -1) {
        throw Socket::SocketError("Write on fd " + this->_socketFd +
                                  (" failed: " + std::string(strerror(errno))));
    }
}

void Socket::writeToSocket(char data) noexcept(false) {
    if (write(this->_socketFd, &data, 1) == -1) {
        throw Socket::SocketError("Write on fd " + this->_socketFd +
                                  (" failed: " + std::string(strerror(errno))));
    }
}

void Socket::writeToSocket(std::vector<uint8_t> buffer) noexcept(false) {
    if (write(this->_socketFd, buffer.data(), buffer.size()) == -1) {
        throw Socket::SocketError("Write on fd " + this->_socketFd +
                                  (" failed: " + std::string(strerror(errno))));
    }
}

std::string Socket::readFromSocket() noexcept(false) {
    char buffer[BUFSIZ];
    int bytes_read = read(this->_socketFd, buffer, sizeof(buffer));

    buffer[bytes_read] = '\0';
    if (bytes_read < 0) {
        throw Socket::SocketError("Read on fd " + this->_socketFd +
                                  (" failed: " + std::string(strerror(errno))));
    }
    return buffer;
}
std::vector<uint8_t> Socket::readFromSocket(size_t size) noexcept(false) {
    std::vector<uint8_t> buffer(size);
    size_t bytes_read = 0;
    while (bytes_read < size) {
        ssize_t result = read(this->_socketFd, buffer.data() + bytes_read,
                              size - bytes_read);
        if (result < 0) {
            throw Socket::SocketError(
                "Read on fd " + std::to_string(this->_socketFd) +
                " failed: " + std::string(strerror(errno)));
        }
        if (result == 0) {
            break;
        }
        bytes_read += result;
    }
    buffer.resize(bytes_read);
    return buffer;
}

struct sockaddr_in Socket::getAddress() const noexcept { return _address; }

void Socket::setCloseOnDestroy(bool closeOnDestroy) noexcept {
    this->_closeSocketOnDestruction = closeOnDestroy;
}
