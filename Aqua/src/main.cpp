#include <chrono>
#include <iomanip>
#include <iostream>

#include "engine.hpp"
#include "vec.hpp"

using namespace std::chrono;

inline double dT(nanoseconds dt) {
    return duration_cast<duration<double, std::milli>>(dt).count();
}
inline double FPS(nanoseconds dt) {
    return 1000 / dT(dt);
}

constexpr double alpha = 0.1;

int main() {
    aq::Engine eng(vec(1920, 1080), 1000, std::time(nullptr));

    eng.startParallelLife();

    double avgFPS = 0;
    auto last_frame = high_resolution_clock::now();

    while (eng.isRunning()) {
        eng.handleEvents();
        eng.draw();
        auto now = high_resolution_clock::now();
        avgFPS = (alpha * FPS(now - last_frame)) + (1.0 - alpha) * avgFPS;
        std::cout << std::setprecision(4) << avgFPS << " FPS" << std::endl;
        last_frame = now;
    }
    return 0;
}