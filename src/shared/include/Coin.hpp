#pragma once

typedef struct coinsPos_s {
    double x_pos;
    double y_pos;


    coinsPos_s(double x = 0.0, double y = 0.0)
        : x_pos(x), y_pos(y) {}
} coinsPos_t;
