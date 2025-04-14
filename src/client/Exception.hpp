//
// Created by roussierenoa on 3/25/25.
//

#ifndef SRC_CLIENT_EXCEPTION_HPP_
#define SRC_CLIENT_EXCEPTION_HPP_

#include <exception>
#include <string>

class NetworkException : public std::exception {
 private:
    std::string _msg;

 public:
    const char *what() const noexcept override { return this->_msg.c_str(); }

    explicit NetworkException(const std::string &msg) : _msg(msg) {}

    ~NetworkException() override = default;
};

class HeaderException : public std::exception {
 private:
    std::string _msg;

 public:
    const char *what() const noexcept override { return this->_msg.c_str(); }

    explicit HeaderException(const std::string &msg) : _msg(msg) {}

    ~HeaderException() override = default;
};

class PayloadException : public std::exception {
 private:
    std::string _msg;

 public:
    const char *what() const noexcept override { return this->_msg.c_str(); }

    explicit PayloadException(const std::string &msg) : _msg(msg) {}

    ~PayloadException() override = default;
};

class ParsingException : public std::exception {
 private:
    std::string _msg;

 public:
    const char *what() const noexcept override { return this->_msg.c_str(); }

    explicit ParsingException(const std::string &msg) : _msg(msg) {}

    ~ParsingException() override = default;
};

class HelpException : public std::exception {
 public:
    HelpException() = default;

    ~HelpException() override = default;
};

#endif  // SRC_CLIENT_EXCEPTION_HPP_
