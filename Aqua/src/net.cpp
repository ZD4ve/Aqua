#include "net.hpp"

#include <cmath>

using namespace aq;

Net::Net(Settings fishSettings, size_t approximateMapSize) : opt(fishSettings) {
    storage.reserve(opt.n_of_fishes);
    std::vector<Force *> tmpF;
    for (size_t i = 0; i < opt.n_of_fishes; i++) {
        // TODO: Fish generation
        size_t vis = 100;
        Fish f = Fish({0, 0}, tmpF, vis);
        storage.push_back(f);

        if (cellSize < vis) cellSize = vis;
    }
    mapSize = size_t(cellSize * round(approximateMapSize / (double)cellSize));
    if (cellCnt() == 0) throw std::logic_error("Fish see farther than map size!");
    grid = new std::forward_list<Fish *> *[cellCnt()];
    grid[0] = new std::forward_list<Fish *>[cellCnt()];
    for (size_t i = 1; i < cellCnt(); i++) {
        grid[i] = grid[i - 1] + cellCnt();
    }
    for (size_t i = 0; i < storage.size(); i++)
    {
        Fish &f = storage[i];
        sf::Vector2u pos = getCord(f);
        grid[pos.x][pos.y].emplace_front(&f);
    }
}

Net::~Net(){
    delete[] grid[0];
    delete[] grid;
}

sf::Vector2u Net::getCord(const Fish &fish) const{
    sf::Vector2f loc = fish.getLocation();
    return sf::Vector2u(loc.x/cellSize,loc.y/cellSize);
}

Net::LocalisedIterator Net::begin(const Fish &centerFish) const{
    return Net::LocalisedIterator(*this, centerFish);
}
