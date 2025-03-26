/*
** EPITECH PROJECT, 2025
** my_jetpack
** File description:
** PollFdList
*/

#ifndef SERVER_SRC_SERVER_POLLFDLIST_HPP_
    #define SERVER_SRC_SERVER_POLLFDLIST_HPP_

    #include <poll.h>
    #include <vector>
    #include <cstdint>

namespace jetpack {
namespace server {
class PollFdList : public std::vector<struct pollfd> {
 public:
    PollFdList() = delete;
    explicit PollFdList(int serverSocket);
    ~PollFdList();
    void addSocket(int socket, int16_t events);
    void removeSocket(int socket);
};
}  // namespace server
}  // namespace jetpack

#endif  // SERVER_SRC_SERVER_POLLFDLIST_HPP_
