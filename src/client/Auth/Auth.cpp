//
// Created by roussierenoa on 4/14/25.
//

#include "Auth.hpp"
#include <string>

void jetpack::Client::Auth::setId(size_t id) {
    this->_authMutex.lock();
    this->_isConnected = true;
    this->_id = id;
    this->_authMutex.unlock();
}

void jetpack::Client::Auth::setUsername(std::string username) {
    this->_username = username;
}

bool jetpack::Client::Auth::isConnected() const {
    return this->_isConnected;
}

int jetpack::Client::Auth::getId() const {
    return this->_id;
}

void jetpack::Client::Auth::resetAuth() {
    this->_username = "default";
    this->_id = 0;
    this->_isConnected = false;
}

std::string jetpack::Client::Auth::getUsername() const {
    return this->_username;
}

jetpack::Client::Auth::Auth(): _id(0) {
}

jetpack::Client::Auth::~Auth() {
}
