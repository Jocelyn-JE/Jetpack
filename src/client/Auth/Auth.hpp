//
// Created by roussierenoa on 4/14/25.
//

#ifndef SRC_CLIENT_AUTH_AUTH_HPP_
#define SRC_CLIENT_AUTH_AUTH_HPP_
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
}  // namespace jetpack::Client

#endif  // SRC_CLIENT_AUTH_AUTH_HPP_
