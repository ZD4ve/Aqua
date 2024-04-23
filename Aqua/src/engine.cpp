#include "engine.hpp"

#include <SFML/Graphics.hpp>
#include <atomic>
#include <thread>

using namespace aq;
using namespace std::chrono;

Engine::Engine(sf::Vector2u window_size, unsigned int seed) : endLife(true) {
    std::srand(seed);

    sf::ContextSettings win_settings;
    win_settings.antialiasingLevel = 8;
    win_settings.majorVersion = 3;
    win_settings.minorVersion = 2;
    sf::VideoMode win_size = sf::VideoMode(window_size.x, window_size.y);
    window = new sf::RenderWindow(win_size, "Aqua by ZDave!", sf::Style::Default, win_settings);
    window->setVerticalSyncEnabled(true);
    window->setFramerateLimit(0);

    island = new Island({1000, 1000});

    Breeder::Settings fish_settings;
    fish_settings.n_of_fishes = 500;
    Breeder::Dependency dependencies;
    dependencies.map = &island->getMap();
    dependencies.mousPos = &mousePosition;
    net = new Net(Breeder(fish_settings, dependencies), 1000);
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
    sf::View view(sf::FloatRect(0.0F, 0.0F, screen.x, screen.y));
    view.setCenter(map.x / 2.0F, map.y / 2.0F);
    double h_ratio = map.x / static_cast<double>(screen.x);
    double v_ratio = map.y / static_cast<double>(screen.y);
    view.zoom(std::max(h_ratio, v_ratio));
    window->setView(view);
}