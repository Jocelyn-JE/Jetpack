/*
** EPITECH PROJECT, 2025
** my_jetpack
** File description:
** Socket
*/

#ifndef SRC_SHARED_SOCKET_SOCKET_HPP_
#define SRC_SHARED_SOCKET_SOCKET_HPP_

#include <netinet/in.h>
#include <unistd.h>

#include <cstring>
#include <exception>
#include <string>
#include <vector>

class Socket {
 public:
    // Default constructor is deleted.
    // You must either open a new socket on a new file descriptor or
    // open a new socket on an existing file descriptor.
    Socket() = delete;

    // Open a new socket on an existing file descriptor specified by fd.
    // This new socket will not be closed on destruction by default.
    Socket(int fd, struct sockaddr_in address) noexcept;

    // Open a new socket on a new file descriptor.
    // This new socket will be closed on destruction by default.
    // May throw a SocketError exception if the socket could not be created.
    Socket(int domain, int type, int protocol) noexcept(false);

    // Close the socket if it was not closed before.
    // May throw a SocketError exception if the socket could not be closed.
    ~Socket() noexcept(false);

    // Open a new socket on a new file descriptor.
    // May throw a SocketError exception if the socket could not be created.
    void resetSocket(int domain, int type, int protocol);

    // Returns the socket's file descriptor.
    int getSocketFd() const noexcept;

    // Returns the socket's address.
    struct sockaddr_in getAddress() const noexcept;

    // Bind the socket to a port.
    // May throw a SocketError exception if the socket could not be bound.
    void bindSocket(uint16_t port) noexcept(false);

    // Listen on the socket.
    // May throw a SocketError exception if the socket could not listen.
    void listenSocket(int backlog) noexcept(false);

    // Connect the socket to an address.
    // May throw a SocketError exception if the socket could not connect.
    void connectSocket(const struct sockaddr *address,
                       socklen_t addressLen) noexcept(false);

    // Connect the socket to a specific IP address and port.
    // May throw a SocketError exception if the socket could not connect.
    void connectSocket(const char *ip_address, uint16_t port) noexcept(false);

    // Write a string to the socket.
    // May throw a SocketError exception if the write failed.
    void writeToSocket(std::string str) noexcept(false);

    // Write a character to the socket.
    // May throw a SocketError exception if the write failed.
    void writeToSocket(char data) noexcept(false);

    // Write a buffer to the socket.
    // May throw a SocketError exception if the write failed.
    void writeToSocket(std::vector<uint8_t> buffer) noexcept(false);

    // Write a buffer to the socket.
    // May throw a SocketError exception if the write failed.
    template <typename T>
    void writeToSocket(T buffer) noexcept(false) {
        if (write(this->_socketFd, &buffer, sizeof(buffer)) == -1) {
            throw Socket::SocketError(
                "Write on fd " + std::to_string(_socketFd) +
                (" failed: " + std::string(strerror(errno))));
        }
    }

    // Write a vector of a buffer to the socket.
    // May throw a SocketError exception if the write failed.
    template <typename T>
    void writeToSocket(std::vector<T> bufferVector) noexcept(false) {
        if (write(this->_socketFd, bufferVector.data(),
                  bufferVector.size() * sizeof(T)) == -1) {
            throw Socket::SocketError(
                "Write on fd " + std::to_string(_socketFd) +
                (" failed: " + std::string(strerror(errno))));
        }
    }

    // Read a string from the socket.
    // May throw a SocketError exception if the read failed.
    std::string readFromSocket() noexcept(false);

    // Read a buffer from the socket.
    // May throw a SocketError exception if the read failed.
    std::vector<uint8_t> readFromSocket(size_t size) noexcept(false);

    // Read a buffer from the socket.
    // May throw a SocketError exception if the read failed.
    template <typename T>
    T readFromSocket() noexcept(false) {
        T buffer;
        if (read(_socketFd, &buffer, sizeof(buffer)) == -1) {
            throw Socket::SocketError(
                "Read on fd " + std::to_string(_socketFd) +
                (" failed: " + std::string(strerror(errno))));
        }
        return buffer;
    }

    // Read a vector of a buffer from the socket.
    // May throw a SocketError exception if the read failed.
    template <typename T>
    std::vector<T> readFromSocket(size_t size) noexcept(false) {
        std::vector<T> buffer(size);
        if (read(_socketFd, buffer.data(), size * sizeof(T)) == -1) {
            throw Socket::SocketError(
                "Read on fd " + std::to_string(_socketFd) +
                (" failed: " + std::string(strerror(errno))));
        }
        return buffer;
    }

    // Close the socket.
    // May throw a SocketError exception if the socket could not be closed.
    void closeSocket() noexcept(false);

    // Returns whether the socket will be closed on destruction.
    bool closesOnDestroy() const noexcept;

    // Set whether the socket will be closed on destruction.
    void setCloseOnDestroy(bool closeOnDestroy) noexcept;

    // Flush the socket by reading all available data.
    void flush() noexcept(false);

    // Exception class for Socket.
    class SocketError : public std::exception {
     public:
        explicit SocketError(std::string message) noexcept;

        ~SocketError() noexcept;

        const char *what() const noexcept override;

     private:
        std::string _message;
    };

 private:
    // Whether the socket (fd) will be closed on destruction.
    bool _closeSocketOnDestruction;
    int _socketFd = -1;
    struct sockaddr_in _address;
};

#endif  // SRC_SHARED_SOCKET_SOCKET_HPP_
