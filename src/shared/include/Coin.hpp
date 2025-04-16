#pragma once

typedef struct coinsPos_s {
    double x_pos;
    double y_pos;

    coinsPos_s(int x = 0, int y = 0) : x_pos(x), y_pos(y) {}
} coinsPos_t;
