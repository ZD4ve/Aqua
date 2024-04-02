#include <SFML/Graphics.hpp>

#include "engine.hpp"

int main() {
    aq::Engine eng(sf::Vector2u(1920,1080));

    eng.startParalellLife();

    while (eng.isRunning()) {
        eng.handeEvents();
        eng.draw();
    }
    return 0;
}