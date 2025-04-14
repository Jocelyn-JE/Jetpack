#pragma once

typedef struct coinsPos_s {
    int x_pos;
    int y_pos;

    coinsPos_s(int x = 0, int y = 0) : x_pos(x), y_pos(y) {}
} coinsPos_t;
