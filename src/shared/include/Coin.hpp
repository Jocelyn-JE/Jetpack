#pragma once

typedef struct coinsPos_s {
    int x_pos;
    int y_pos;
    size_t coinId;

    coinsPos_s(int x = 0, int y = 0, size_t id = 0)
        : x_pos(x), y_pos(y), coinId(id) {}
} coinsPos_t;
