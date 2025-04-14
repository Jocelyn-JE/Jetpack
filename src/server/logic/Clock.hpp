#pragma once
#include <chrono>

class Clock {
 private:
    std::chrono::time_point<std::chrono::high_resolution_clock> lastTick;
    float deltaTime;

 public:
    Clock();
    float tick();
    float getDeltaTime() const;
};
