/*
** EPITECH PROJECT, 2025
** my_jetpack
** File description:
** Socket
*/

#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>
#include <string>
#include <vector>

#include "Socket.hpp"

Socket::Socket(int fd, struct sockaddr_in address) noexcept
    : _closeSocketOnDestruction(false) {
    this->_socketFd = fd;
    this->_address = address;
    std::cout << "Fetched new socket on fd: " << this->_socketFd << std::endl;
}

Socket::Socket(int domain, int type, int protocol) noexcept(false)
    : _closeSocketOnDestruction(true) {
    this->_socketFd = socket(domain, type, protocol);
    if (this->_socketFd == -1) {
        throw Socket::SocketError("Socket creation failed: " +
                                  std::string(strerror(errno)));
    }
    std::cout << "Created socket on fd: " << this->_socketFd << std::endl;
}

Socket::~Socket() noexcept(false) {
    if (this->_socketFd >= 0 && this->_closeSocketOnDestruction) {
        if (close(this->_socketFd) == -1) {
            throw Socket::SocketError("Failed to close socket: " +
                                      std::string(strerror(errno)));
        }
        std::cout << "Closed socket on fd: " << this->_socketFd << std::endl;
    }
    std::cout << "Removed socket on fd " << this->_socketFd << " from server"
              << std::endl;
}

void Socket::resetSocket(int domain, int type, int protocol) {
    if (this->_socketFd != -1) return;
    this->_socketFd = socket(domain, type, protocol);
    if (this->_socketFd == -1) {
        throw Socket::SocketError("Socket creation failed: " +
                                  std::string(strerror(errno)));
    }
    std::cout << "Created socket on fd: " << this->_socketFd << std::endl;
}

bool Socket::closesOnDestroy() const noexcept {
    return this->_closeSocketOnDestruction;
}

void Socket::closeSocket() noexcept(false) {
    if (this->_socketFd == -1) {
        std::cout << "Socket already closed on fd: " << this->_socketFd
                  << std::endl;
        return;
    }
    if (close(this->_socketFd) == -1) {
        throw Socket::SocketError("Failed to close socket: " +
                                  std::string(strerror(errno)));
    }
    std::cout << "Closed socket on fd: " << this->_socketFd << std::endl;
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
    
    // Déterminer si le socket est non-bloquant
    int flags = fcntl(this->_socketFd, F_GETFL, 0);
    bool is_nonblocking = (flags & O_NONBLOCK);
    
    // Si c'est un socket non-bloquant, passer temporairement en mode bloquant avec timeout
    if (is_nonblocking) {
        // Mettre en mode bloquant temporairement
        fcntl(this->_socketFd, F_SETFL, flags & ~O_NONBLOCK);
        
        // Définir un timeout pour la connexion
        struct timeval tv;
        tv.tv_sec = 3;  // 3 secondes timeout
        tv.tv_usec = 0;
        setsockopt(this->_socketFd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
        setsockopt(this->_socketFd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&tv, sizeof tv);
    }
    
    // Tentative de connexion
    int connect_result = connect(this->_socketFd, (const struct sockaddr *)&address, sizeof(address));
    
    // Restaurer le mode non-bloquant si nécessaire
    if (is_nonblocking) {
        fcntl(this->_socketFd, F_SETFL, flags);
    }
    
    // Gérer les erreurs de connexion
    if (connect_result == -1) {
        // Si c'est un socket non-bloquant et que l'erreur est EINPROGRESS, c'est normal
        if (is_nonblocking && errno == EINPROGRESS) {
            std::cout << "Connection in progress on fd: " << this->_socketFd << std::endl;
            return;
        }
        throw Socket::SocketError("Connect failed to " + std::string(ip_address) + 
                                 ":" + std::to_string(port) + " - " +
                                  std::string(strerror(errno)));
    }
    
    std::cout << "Successfully connected to " << ip_address << ":" << port << " on fd: " << this->_socketFd << std::endl;
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
    int bytes_read = read(this->_socketFd, buffer.data(), buffer.size());

    if (bytes_read < 0) {
        throw Socket::SocketError("Read on fd " + this->_socketFd +
                                  (" failed: " + std::string(strerror(errno))));
    }
    return buffer;
}

struct sockaddr_in Socket::getAddress() const noexcept { return _address; }

void Socket::setCloseOnDestroy(bool closeOnDestroy) noexcept {
    this->_closeSocketOnDestruction = closeOnDestroy;
}
