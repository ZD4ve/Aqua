#include "net.hpp"

#include <cmath>

using namespace aq;

Net::Net(Settings fishSettings, size_t approximateMapSize) : opt(fishSettings) {
    storage = new Fish[opt.n_of_fishes];
    std::vector<Force *> tmpF;
    for (size_t i = 0; i < opt.n_of_fishes; i++) {
        // TODO: Fish generation
        size_t vis = 100;
        Fish f = Fish({0, 0}, tmpF, vis);
        storage[i] = f;

        if (cellSize < vis) cellSize = vis;
    }
    mapSize = size_t(cellSize * round(approximateMapSize / (double)cellSize));
    if (cellCnt() == 0) throw std::logic_error("Fish see farther than map size!");
    grid = new Net::cell *[cellCnt() + 2];
    grid[0] = new Net::cell[(cellCnt() + 2) * (cellCnt() + 2)];
    for (size_t i = 1; i < cellCnt() + 2; i++) {
        grid[i] = grid[i - 1] + cellCnt() + 2;
    }
    for (size_t i = 0; i < opt.n_of_fishes; i++) {
        Fish &f = storage[i];
        sf::Vector2u pos = getCord(f);
        grid[pos.x+1][pos.y+1].emplace_front(&f);
    }
}

Net::~Net() {
    delete[] storage;
    delete[] grid[0];
    delete[] grid;
}

sf::Vector2u Net::getCord(const Fish &fish) const {
    sf::Vector2f loc = fish.getLocation();
    return sf::Vector2u(loc.x / cellSize, loc.y / cellSize);
}

Net::LocalisedIterator Net::begin(const Fish &centerFish) const {
    return Net::LocalisedIterator(*this, centerFish);
}
const Net::LocalisedIterator Net::cend(const Fish &centerFish) const {
    auto iter = Net::LocalisedIterator(*this, centerFish);
    iter.gotoEnd();
    return iter;
}
