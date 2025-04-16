//
// Created by roussierenoa on 4/14/25.
//

#ifndef SRC_CLIENT_AUTH_AUTHGRAPHIC_HPP_
#define SRC_CLIENT_AUTH_AUTHGRAPHIC_HPP_
#include <mutex>
#include <string>

namespace jetpack::Client {
class Auth {
 private:
    std::mutex _authMutex;
    bool _isConnected = false;
    size_t _id;
    std::string _username = "default";

 public:
    void setId(size_t id);

    void setUsername(std::string username);

    bool isConnected();

    int getId();

    std::string getUsername() const;

    void resetAuth();

    Auth();

    ~Auth();
};
}  // namespace jetpack::Client

#endif  // SRC_CLIENT_AUTH_AUTHGRAPHIC_HPP_
