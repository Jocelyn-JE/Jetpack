//
// Created by roussierenoa on 3/28/25.
//

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>

namespace Jetpack {

class Logger {
private:
	bool _isDebugMode = false;
	std::mutex _mutex;
public:
	void log(const std::string &message) {
		if (!this->_isDebugMode)
			return;
		this->_mutex.lock();
		std::cout << "[DEBUG] " << message << std::endl;
		this->_mutex.unlock();
	}
    Logger(bool isDebug) : _isDebugMode(isDebug) {};
};

} // Jetpack

#endif //LOGGER_HPP
