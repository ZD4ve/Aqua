#include "breeder.hpp"

#include "forces.hpp"

using namespace aq;

Breeder::Breeder(Settings fishSettings) : opt(fishSettings) {}

Fish *Breeder::make() {
    Fish *storage = new Fish[opt.n_of_fishes];
    std::vector<Force *> forces;
    forces.push_back(new SeparationForce(0.2, 10));
    forces.push_back(new AlignmentForce(0.8));
    forces.push_back(new CohesionForce(0.8));
    forces.push_back(new WaterResistanteForce(0.001));
    forces.push_back(new MinSpeedForce(10));

    for (size_t i = 0; i < opt.n_of_fishes; i++) {
        // TODO: Fish generation
        float vis = 50;
        Fish fish = Fish(sf::Vector2f(rand() % 1000, rand() % 1000), forces, vis, sf::Color(rand() % 255, rand() % 255, rand() % 255));
        storage[i] = fish;

        if (max_vision < vis) max_vision = vis;
    }
    for (auto &f : forces) {
        delete f;
    }
    return storage;
};
