#include <chrono>
#include <iomanip>
#include <iostream>

#include "engine.hpp"
#include "vec.hpp"

/**
 * @details
 * Feel free to experiment with different number of fishes, based on your systems performance.
 * On my machine it can run 3x of fish in Release.
 */
#define NUMBER_OF_FISH 3000
// #define FPS_COUNTER

#ifdef FPS_COUNTER
using namespace std::chrono;
inline double dT(nanoseconds dt) {
    return duration_cast<duration<double, std::milli>>(dt).count();
}
inline double FPS(nanoseconds dt) {
    return 1000 / dT(dt);
}
constexpr double alpha = 0.01;
#endif

int main() {
    aq::Engine eng(vec(1920, 1080), NUMBER_OF_FISH, 0);  // std::time(nullptr)

    eng.startParallelLife();

#ifdef FPS_COUNTER
    double avgFPS = 0;
    auto last_frame = high_resolution_clock::now();
    auto last_print = high_resolution_clock::now();
#endif

    while (eng.isRunning()) {
        eng.handleEvents();
        eng.draw();
#ifdef FPS_COUNTER
        auto now = high_resolution_clock::now();
        avgFPS = (alpha * FPS(now - last_frame)) + (1.0 - alpha) * avgFPS;
        if (dT(now - last_print) > 100) {
            std::cout << std::setprecision(4) << avgFPS << " FPS\n";
            last_print = now;
        }
        last_frame = now;
#endif
    }
    return 0;
}