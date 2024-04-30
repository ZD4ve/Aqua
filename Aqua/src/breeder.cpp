#include "breeder.hpp"

#include "color.hpp"
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
    forces.push_back(new SeparationForce(0.2, 7));
    forces.push_back(new AlignmentForce(0.8));
    forces.push_back(new CohesionForce(0.8));
    forces.push_back(new WaterResistanteForce(0.01));
    forces.push_back(new MinSpeedForce(10));
    forces.push_back(new MouseForce(0.5, 30, *dep.mousPos));
    forces.push_back(new IslandForce(0.15, *dep.map));

    for (size_t i = 0; i < opt.n_of_fishes; i++) {
        Color col = Color::randomColor(45, 45, .9, .5, 20);
        // TODO: Fish generation
        float vis = 20;
        vec pos;
        do {
            pos.x = std::rand() % static_cast<size_t>(opt.mapSize.x);
            pos.y = std::rand() % static_cast<size_t>(opt.mapSize.y);
        } while (!dep.map->waterAt(pos));

        Fish fish = Fish(pos, forces, vis, col);
        storage[i] = fish;

        if (max_vision < vis) max_vision = vis;
    }
    for (auto &f : forces) {
        delete f;
    }
    return storage;
};
