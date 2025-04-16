#pragma once

typedef struct coinsPos_s {
    double x_pos;
    double y_pos;
    size_t coinId;

    coinsPos_s(double x = 0.0, double y = 0.0, size_t id = 0)
        : x_pos(x), y_pos(y), coinId(id) {}
} coinsPos_t;
