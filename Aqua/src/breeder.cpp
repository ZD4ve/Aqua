#include "breeder.hpp"

#include "color.hpp"
#include "forces.hpp"

using namespace aq;

Breeder::Breeder(Settings fishSettings, Dependency forceDependencies) : opt(fishSettings), dep(forceDependencies) {
    if (!dep.isSet()) {
        throw std::logic_error("Force dependencies hasn't been set correctly!");
    }
}

Fish *Breeder::make() {
    Fish *storage = new Fish[opt.n_of_fishes];
    std::vector<Force *> forces;
    forces.push_back(new SeparationForce(0.2, 7));
    forces.push_back(new AlignmentForce(0.8));
    forces.push_back(new CohesionForce(0.2));
    forces.push_back(new SpeciesCohesionForce(0.6));
    forces.push_back(new WaterResistanceForce(0.01));
    forces.push_back(new MinSpeedForce(10));
    forces.push_back(new MouseForce(0.5, 30, *dep.mousePos));
    forces.push_back(new IslandForce(0.15, *dep.map));

    if (opt.n_of_fishes % opt.n_of_species != 0) {
        throw std::logic_error("Fish count must be a multiple of species count!");
    }

    for (size_t s = 0; s < opt.n_of_species; s++) {
        Color col = Color::randomColor(.8, .65, 15);
        float vis = 20;
        vec spawn;
        do {
            spawn.x = static_cast<double>(std::rand() % static_cast<size_t>(opt.mapSize.x));
            spawn.y = static_cast<double>(std::rand() % static_cast<size_t>(opt.mapSize.y));
        } while (!dep.map->waterAt(spawn));
        for (size_t i = 0; i < opt.n_of_fishes / opt.n_of_species; i++) {
            vec pos;
            size_t range = 100;
            do {
                pos = spawn + vec(std::rand() % range - range / 2, std::rand() % range - range / 2);
                range = std::min(static_cast<size_t>(static_cast<double>(range) * 1.05), 500UL);
            } while (!dep.map->waterAt(pos));

            Fish fish = Fish(pos, forces, vis, col, s);
            storage[s * opt.n_of_fishes / opt.n_of_species + i] = fish;

            if (max_vision < vis) max_vision = vis;
        }
    }

    for (auto &f : forces) {
        delete f;
    }
    return storage;
};
