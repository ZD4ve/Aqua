#include "engine.hpp"

#include <SFML/Graphics.hpp>

using namespace aq;
using namespace std::chrono;

Engine::Engine(vec window_size, size_t fish_number, unsigned int seed) {
    std::srand(seed);

    sf::ContextSettings win_settings;
    win_settings.antialiasingLevel = 4;
    win_settings.majorVersion = 3;
    win_settings.minorVersion = 2;
    sf::VideoMode win_size = sf::VideoMode(window_size.x, window_size.y);
    window = new sf::RenderWindow(win_size, "Aqua by ZDave", sf::Style::Default, win_settings);
    window->setVerticalSyncEnabled(true);
    window->setFramerateLimit(0);

    island = new Island(vec(1000, 1000));

    Breeder::Settings fish_settings;
    fish_settings.n_of_fishes = fish_number;
    fish_settings.n_of_species = 10;
    fish_settings.mapSize = island->getMapSize();
    Breeder::Dependency dependencies;
    dependencies.map = &island->getMap();
    dependencies.mousePos = &mousePosition;
    net = new Net(Breeder(fish_settings, dependencies), 1000);
}

Engine::~Engine() {
    window->close();
    bgLife.request_stop();
    bgLife.join();
    delete window;
    delete net;
    delete island;
}
void Engine::startParallelLife() {
    bgLife = std::jthread(std::bind_front(&Net::moveFishWhile, this->net));
}
void Engine::draw() {
    window->clear(island->getBGcolor());

    island->draw(*window);
    net->draw(*window);

    window->display();
}

// src: https://github.com/SFML/SFML/wiki/Source:-Zoom-View-At-(specified-pixel)
void Engine::zoomViewAt(vec pixel, bool in) {
    float zoom = in ? zoomAmount : 1 / zoomAmount;
    const vec beforeCoord = window->mapPixelToCoords(pixel);
    sf::View view = window->getView();
    view.zoom(zoom);
    window->setView(view);
    const vec afterCoord = window->mapPixelToCoords(pixel);
    const vec offsetCoords = beforeCoord - afterCoord;
    view.move(offsetCoords);
    window->setView(view);
}
void Engine::resetView() {
    vec map = island->getMapSize();
    vec screen = window->getSize();
    sf::View view(sf::FloatRect(0.0F, 0.0F, screen.x, screen.y));
    view.setCenter(map.x / 2.0F, map.y / 2.0F);
    double h_ratio = map.x / screen.x;
    double v_ratio = map.y / screen.y;
    view.zoom(std::min(h_ratio, v_ratio) * 0.8);
    window->setView(view);
}