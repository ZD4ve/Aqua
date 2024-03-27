#include <SFML/Graphics.hpp>


#define window_width 1920
#define window_height 1080

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    auto window = sf::RenderWindow({window_width, window_height}, "Mukodj!", sf::Style::Default, settings);
    window.setFramerateLimit(0);
    window.setVerticalSyncEnabled(true);
    while (window.isOpen()) {
        for (auto event = sf::Event{}; window.pollEvent(event);) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            window.clear();
            window.display();
        }
    }
    window.close();
    return 0;
}