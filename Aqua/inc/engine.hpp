#pragma once
#include <SFML/Graphics.hpp>
#include <thread>

#include "island.hpp"
#include "net.hpp"
#include "vec.hpp"

namespace aq {

/**
 * @brief The game engine setting up, running and shutting down the game
 */
class Engine {
   private:
    sf::RenderWindow *window;
    Net *net;
    Island *island;
    const float zoomAmount = 1.3F;
    std::jthread bgLife;
    std::atomic<sf::Vector2f> mousePosition;  ///< The position of the mouse for objects that cannot access the window
    void zoomViewAt(vec pixel, bool in);
    void resetView();

   public:
    explicit Engine(vec window_size, size_t fish_number, unsigned int seed);
    void draw();
    /**
     * @brief Starts the background process for the calculations
     */
    void startParallelLife();
    /**
     * @brief Stops the background process for the calculations
     */
    bool isRunning() const {
        return window->isOpen();
    }
    void handleEvents();
    ~Engine();
};

}  // namespace aq