#include "engine.hpp"

#include <SFML/Graphics.hpp>
#include <atomic>
#include <thread>

using namespace aq;
using namespace std::chrono;

Engine::Engine(sf::Vector2u window_size) : endLife(true) {
    sf::ContextSettings win_settings;
    // win_settings.antialiasingLevel = 8;
    win_settings.majorVersion = 3;
    win_settings.minorVersion = 2;
    sf::VideoMode win_size = sf::VideoMode(window_size.x, window_size.y);
    window = new sf::RenderWindow(win_size, "Aqua by ZDave!", sf::Style::Default, win_settings);
    window->setVerticalSyncEnabled(true);
    window->setFramerateLimit(0);

    island = new Island({1000, 1000});

    Breeder::Settings fish_settings;
    net = new Net(Breeder(fish_settings), 1000);
}

Engine::~Engine() {
    if (!endLife) stopParalellLife();
    window->close();
    delete window;
    delete net;
    delete island;
}
void Engine::startParalellLife() {
    if (!endLife) throw std::logic_error("Life already running!");
    endLife = false;
    bgLife = std::thread(&Engine::life, this);
}
void Engine::stopParalellLife() {
    if (endLife) throw std::logic_error("Life already stopped!");
    endLife = true;
    bgLife.join();
}
void Engine::life() {
    while (!endLife) {
        net->moveFish();
        std::this_thread::sleep_for(1ns);
    }
}
void Engine::draw() {
    window->clear(sf::Color(10, 44, 53));

    island->draw(*window);
    net->draw(*window);

    window->display();
}

// src: https://github.com/SFML/SFML/wiki/Source:-Zoom-View-At-(specified-pixel)
void Engine::zoomViewAt(sf::Vector2i pixel, bool in) {
    float zoom = in ? zoomAmount : 1 / zoomAmount;
    const sf::Vector2f beforeCoord = window->mapPixelToCoords(pixel);
    sf::View view = window->getView();
    view.zoom(zoom);
    window->setView(view);
    const sf::Vector2f afterCoord = window->mapPixelToCoords(pixel);
    const sf::Vector2f offsetCoords = beforeCoord - afterCoord;
    view.move(offsetCoords);
    window->setView(view);
}
void Engine::resetView() {
    sf::Vector2u map = island->getMapSize();
    sf::Vector2u screen = window->getSize();
    sf::View view(sf::FloatRect(0.f, 0.f, screen.x, screen.y));
    view.setCenter(map.x / 2.f, map.y / 2.f);
    double h_ratio = map.x / double(screen.x);
    double v_ratio = map.y / double(screen.y);
    view.zoom(std::max(h_ratio, v_ratio));
    window->setView(view);
}

void Engine::handeEvents() {
    static bool mouse_down = false;
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
                mouse_down = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
                last_pos = sf::Mouse::getPosition(*window);
                break;
            }
            case Event::MouseMoved: {
                if (!mouse_down) break;
                sf::View view = window->getView();
                sf::Vector2i current_pos = {event.mouseMove.x, event.mouseMove.y};
                view.move(window->mapPixelToCoords(last_pos) - window->mapPixelToCoords(current_pos));
                last_pos = current_pos;
                window->setView(view);
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