#include <cmath>

#include "engine.hpp"
using namespace aq;

void Engine::handeEvents() {
    static bool right_mouse_down = false;
    static bool left_mouse_down = false;

    static sf::Vector2i last_pos;

    for (sf::Event event{}; window->pollEvent(event);) {
        using sf::Event;
        switch (event.type) {
            case Event::Closed: {
                window->close();
                break;
            }
            case Event::MouseButtonPressed:
            case Event::MouseButtonReleased: {
                right_mouse_down = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
                left_mouse_down = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
                //if(left_mouse_down) mousePosition = left_mouse_down ? current_pixel : sf::Vector2f(0, 0);

                break;
            }
            case Event::MouseMoved: {
                sf::Vector2i current_pos = {event.mouseMove.x, event.mouseMove.y};
                sf::View view = window->getView();
                sf::Vector2f current_pixel = window->mapPixelToCoords(current_pos);
                mousePosition = left_mouse_down ? current_pixel : sf::Vector2f(0, 0);
                if (right_mouse_down) {
                    view.move(window->mapPixelToCoords(last_pos) - current_pixel);
                    window->setView(view);
                }
                last_pos = current_pos;
                break;
            }
            case Event::MouseWheelScrolled: {
                zoomViewAt({event.mouseWheelScroll.x, event.mouseWheelScroll.y}, event.mouseWheelScroll.delta < 0);
                break;
            }
            case Event::KeyPressed: {
                using sf::Keyboard;
                switch (event.key.code) {
                    case sf::Keyboard::R:
                        resetView();
                        break;
                    default:
                        break;
                }
                break;
            }
            case Event::Resized: {
                resetView();
                break;
            }
            default:
                break;
        }
    }
}