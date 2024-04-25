#include "breeder.hpp"

#include "forces.hpp"

using namespace aq;

Breeder::Breeder(Settings fishSettings, Dependency forceDependecies) : opt(fishSettings), dep(forceDependecies) {
    if (!dep.isSet()) {
        throw std::logic_error("Force dependencies hasnt been set correctly!");
    }
}

Fish *Breeder::make() {
    Fish *storage = new Fish[opt.n_of_fishes];
    std::vector<Force *> forces;
    forces.push_back(new SeparationForce(0.2, 10));
    forces.push_back(new AlignmentForce(0.8));
    forces.push_back(new CohesionForce(0.8));
    forces.push_back(new WaterResistanteForce(0.01));
    forces.push_back(new MinSpeedForce(10));
    forces.push_back(new MouseForce(0.5, 30, *dep.mousPos));
    forces.push_back(new IslandForce(0.15, *dep.map));

    for (size_t i = 0; i < opt.n_of_fishes; i++) {
        // TODO: Fish generation
        float vis = 20;
        Fish fish = Fish(vec(std::rand() % 500, std::rand() % 500) + vec(250, 250), forces, vis, sf::Color(std::rand() % 255, std::rand() % 255, std::rand() % 255));
        storage[i] = fish;

        if (max_vision < vis) max_vision = vis;
    }
    for (auto &f : forces) {
        delete f;
    }
    return storage;
};
