#include "engine.hpp"

#include <SFML/Graphics.hpp>
#include <atomic>
#include <thread>

using namespace aq;

Engine::Engine(sf::Vector2u window_size) : endLife(true) {
    sf::ContextSettings win_settings;
    win_settings.antialiasingLevel = 8;
    win_settings.majorVersion = 3;
    win_settings.minorVersion = 2;
    sf::VideoMode win_size = sf::VideoMode(window_size.x, window_size.y);
    window = new sf::RenderWindow(win_size, "Aqua by ZDave!", sf::Style::Default, win_settings);
    window->setVerticalSyncEnabled(true);
    window->setFramerateLimit(0);

    island = new Island({1000, 1000});

    Net::Settings fish_settings;
    net = new Net(fish_settings, 1000);
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
    }
}
void Engine::draw() {
    window->clear(sf::Color::White);

    island->draw(*window);
    net->draw(*window);

    window->display();
}
void Engine::handeEvents() {
    auto &win = *window;
    for (auto event = sf::Event{}; win.pollEvent(event);) {
        switch (event.type) {
            case sf::Event::Closed:
                win.close();
                break;
            default:
                break;
        }
    }
}