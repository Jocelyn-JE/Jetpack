//
// Created by roussierenoa on 4/14/25.
//

#ifndef AUTH_HPP
#define AUTH_HPP
#include <string>

namespace jetpack::Client {
class Auth {
 private:
    bool _isConnected = false;
    size_t _id;
    std::string _username = "default";

 public:

    void setId(size_t id);

    void setUsername(std::string username);

    bool isConnected() const;

    int getId() const;

    std::string getUsername() const;

    Auth();
    ~Auth();
};
} // jetpack

#endif //AUTH_HPP
