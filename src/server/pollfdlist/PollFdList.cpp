/*
** EPITECH PROJECT, 2025
** my_jetpack
** File description:
** PollFdList
*/

#include "PollFdList.hpp"

jetpack::server::PollFdList::PollFdList(int serverSocket) {
    addSocket(serverSocket, POLLIN);
}

jetpack::server::PollFdList::~PollFdList() {}

// Socket is the file descriptor to poll, events are the events that are polled
void jetpack::server::PollFdList::addSocket(int socket, int16_t events) {
    this->push_back({.fd = socket, .events = events, .revents = 0});
}

// Socket is the file descriptor to remove from the list
void jetpack::server::PollFdList::removeSocket(int socketFd) {
    for (std::size_t i = 0; i < this->size(); i++) {
        if ((*this)[i].fd == socketFd) {
            this->erase(this->begin() + i);
        }
    }
}
