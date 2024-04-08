#include "breeder.hpp"

using namespace aq;

Breeder::Breeder(Settings fishSettings) : opt(fishSettings) {}

Fish *Breeder::make() {
    Fish *storage = new Fish[opt.n_of_fishes];
    std::vector<Force *> tmpF;
    for (size_t i = 0; i < opt.n_of_fishes; i++) {
        // TODO: Fish generation
        float vis = 100;
        Fish fish = Fish({100, 100}, tmpF, vis, sf::Color::Red);
        storage[i] = fish;

        if (max_vision < vis) max_vision = vis;
    }
    return storage;
};
