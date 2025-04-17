#pragma once

typedef struct obstacle_s {
    double x_pos;
    double y_pos;

    obstacle_s(double x = 0.0, double y = 0.0) : x_pos(x), y_pos(y) {}
} obstacle_t;
