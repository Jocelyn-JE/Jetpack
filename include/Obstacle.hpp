#pragma once

typedef struct obstacle_s {
    int x_pos;
    int y_pos;

    obstacle_s(int x = 0, int y = 0) : x_pos(x), y_pos(y) {}
} obstacle_t;
