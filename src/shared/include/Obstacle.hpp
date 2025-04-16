#pragma once

typedef struct obstacle_s {
    double x_pos;
    double y_pos;

    obstacle_s(int x = 0, int y = 0) : x_pos(x), y_pos(y) {}
} obstacle_t;
