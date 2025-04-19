#pragma once
#include <string>
#include <cstring>

typedef struct player {
    int id;
    char username[20];
    float y_pos;
    int coins_collected;
    bool is_dead;
    bool is_jetpack_on;
    bool host;
} player_t;
