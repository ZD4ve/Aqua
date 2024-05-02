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

    if (opt.n_of_fishes % opt.n_of_species != 0) {
        throw std::logic_error("Fish count must be a multiple of species count!");
    }

    for (size_t s = 0; s < opt.n_of_species; s++) {
        Color col = Color::randomColor(45, 90, .9, .5, 10);
        float vis = 20;
        vec spawn;
        do {
            spawn.x = std::rand() % static_cast<size_t>(opt.mapSize.x);
            spawn.y = std::rand() % static_cast<size_t>(opt.mapSize.y);
        } while (!dep.map->waterAt(spawn));
        for (size_t i = 0; i < opt.n_of_fishes / opt.n_of_species; i++) {
            vec pos;
            int range = 100;
            do {
                pos.x = spawn.x + std::rand() % range - range / 2;
                pos.y = spawn.y + std::rand() % range - range / 2;
                range *= 1.2;
            } while (!dep.map->waterAt(pos));

            Fish fish = Fish(pos, forces, vis, col);
            storage[s * opt.n_of_fishes / opt.n_of_species + i] = fish;

            if (max_vision < vis) max_vision = vis;
        }
    }

    for (auto &f : forces) {
        delete f;
    }
    return storage;
};
