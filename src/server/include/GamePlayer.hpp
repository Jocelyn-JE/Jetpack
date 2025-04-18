#pragma once
#include <cstring>
#include <memory>
#include <string>
#include <vector>

#include "Coin.hpp"

typedef struct gameplayer {
    int id;
    char username[20];
    float y_pos;
    float velocity;
    int coins_collected;
    bool is_dead;
    bool is_jetpack_on;
    bool host;
    std::vector<std::shared_ptr<coinsPos_t>> coins;

    gameplayer(int _id = 0, const std::string &_username = "")
        : id(_id),
          y_pos(0.0),
          velocity(0.0),
          coins_collected(0),
          is_dead(false),
          is_jetpack_on(false),
          host(false),
          coins() {
        std::strncpy(username, _username.c_str(), sizeof(username) - 1);
        username[sizeof(username) - 1] = '\0';
    }
} gameplayer_t;
