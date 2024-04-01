#include <SFML/Graphics.hpp>
#include "net.hpp"
#include "island.hpp"

#define window_width 1920
#define window_height 1080

using namespace aq;

int main() {
    sf::ContextSettings win_settings;
    win_settings.antialiasingLevel = 8;
    auto window = sf::RenderWindow({window_width, window_height}, "Aqua by ZDave!", sf::Style::Default, win_settings);
    window.setFramerateLimit(0);
    window.setVerticalSyncEnabled(true);

    Island sea(sf::Vector2u(1000,1000));
    




    while (window.isOpen()) {
        for (auto event = sf::Event{}; window.pollEvent(event);) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            window.clear();

            sea.draw(window);





            window.display();
        }
    }
    window.close();
    return 0;
}