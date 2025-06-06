//
// Created by roussierenoa on 3/28/25.
//

#ifndef SRC_SHARED_UTILITY_LOGGER_HPP_
#define SRC_SHARED_UTILITY_LOGGER_HPP_

#include <iostream>
#include <mutex>
#include <string>

namespace jetpack {
class Logger {
 private:
    bool _isDebugMode = false;
    std::mutex _mutex;

 public:
    void log(const std::string &message) {
        if (!this->_isDebugMode) return;
        this->_mutex.lock();
        std::cerr << "[DEBUG] " << message << std::endl;
        this->_mutex.unlock();
    }

    explicit Logger(bool isDebug) : _isDebugMode(isDebug) {}
};
}  // namespace jetpack

#endif  // SRC_SHARED_UTILITY_LOGGER_HPP_
