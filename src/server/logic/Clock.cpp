#include "Clock.hpp"

Clock::Clock()
    : lastTick(std::chrono::high_resolution_clock::now()), deltaTime(0.0f) {}

float Clock::tick() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    deltaTime = std::chrono::duration<float>(currentTime - lastTick).count();
    lastTick = currentTime;
    return deltaTime;
}

float Clock::getDeltaTime() const { return deltaTime; }
