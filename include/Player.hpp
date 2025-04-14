#pragma once
#include <cstring>
#include <string>

typedef struct player {
    int id;
    char username[20];
    double y_pos;
    double velocity;
    int coins_collected;
    bool is_dead;
    bool is_jetpack_on;
    bool host;

    player(int _id = 0, const std::string& _username = "")
        : id(_id),
          y_pos(0.0),
          velocity(0.0),
          coins_collected(0),
          is_dead(false),
          is_jetpack_on(false),
          host(false) {
        std::strncpy(username, _username.c_str(), sizeof(username) - 1);
        username[sizeof(username) - 1] = '\0';
    }
} player_t;
