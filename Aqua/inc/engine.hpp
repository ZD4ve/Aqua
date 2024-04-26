#pragma once
#include <SFML/Graphics.hpp>
#include <atomic>
#include <thread>

#include "island.hpp"
#include "net.hpp"
#include "vec.hpp"

namespace aq {

class Engine {
   private:
    sf::RenderWindow *window;
    Net *net;
    Island *island;
    std::atomic_bool live{false};
    const float zoomAmount = 1.3F;
    std::thread bgLife;
    std::atomic<sf::Vector2f> mousePosition;
    void zoomViewAt(vec pixel, bool in);
    void resetView();

   public:
    explicit Engine(vec window_size, size_t fish_number, unsigned int seed);
    void draw();
    void startParalellLife();
    void stopParalellLife();
    bool isRunning() {
        return window->isOpen();
    }
    void handeEvents();
    ~Engine();
};

}  // namespace aq