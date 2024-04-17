#pragma once
#include <SFML/Graphics.hpp>
#include <atomic>
#include <thread>

#include "island.hpp"
#include "net.hpp"

namespace aq {

class Engine {
   private:
    sf::RenderWindow *window;
    Net *net;
    Island *island;
    std::atomic<bool> endLife;
    const float zoomAmount = 1.3F;
    std::thread bgLife;
    void zoomViewAt(sf::Vector2i pixel, bool in);
    void resetView();
    void life();

   public:
    explicit Engine(sf::Vector2u window_size);
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