#include "net.hpp"

#include <cmath>

using namespace aq;
using namespace std::chrono;

// #define PARAL

Net::Net(Breeder breeder, size_t mapSize) : fish_cnt(breeder.getCnt()), mapSize(mapSize) {
    storage = breeder.make();
    size_t cnt = std::floor(mapSize / breeder.getMaxVision()) / 3;
    if (cnt == 0) throw std::logic_error("Fish see farther than map size!");
    cellSize = mapSize / static_cast<float>(cnt);
    cellCnt = mapSize / cellSize;
    grid = new Net::cell *[cellCnt + 2];
    grid[0] = new Net::cell[(cellCnt + 2) * (cellCnt + 2)];
    for (size_t i = 1; i < cellCnt + 2; i++) {
        grid[i] = grid[i - 1] + cellCnt + 2;
    }
    for (size_t i = 0; i < fish_cnt; i++) {
        at(getCord(storage[i])).push_back(&storage[i]);
    }
}

void Net::moveFish() {
    working.lock();
    sf::Time deltaT = lastUpdate.restart();

#ifdef PARAL
    // #pragma omp parallel
    for (size_t j = 0; j < 9; j++) {
        // #pragma omp for
        for (size_t i = j; i < cellCnt * cellCnt; i += 9) {
            vec cord(i % cellCnt, i / cellCnt);

            cell current_cell = at(cord);
            for (cell::iterator iter = current_cell.begin(); iter != current_cell.end();) {
                Fish &fish = **iter;

                fish.move(deltaT, begin(fish), end(fish));

                vec new_cord = getCord(fish);
                if (!new_cord.wholeEQ(cord)) {
                    at(new_cord).push_back(*iter);
                    iter = current_cell.erase(iter);
                } else {
                    ++iter;
                }
            }
        }
    }
#endif
#ifndef PARAL
    for (size_t i = 0; i < fish_cnt; i++) {
        storage[i].move(deltaT, storage, storage + fish_cnt);
    }
#endif
    working.unlock();
}
void Net::draw(sf::RenderTarget &target) {
    working.lock();
    for (size_t i = 0; i < fish_cnt; i++) {
        storage[i].draw(target);
    }
    working.unlock();
}
Net::~Net() {
    delete[] storage;
    delete[] grid[0];
    delete[] grid;
}

vec Net::getCord(const Fish &fish) const {
    vec loc = fish.getLocation();
    return loc / cellSize;
}

Net::LocalisedIterator Net::begin(const Fish &centerFish) {
    return Net::LocalisedIterator(*this, centerFish);
}
Net::LocalisedIterator Net::end(const Fish &centerFish) {
    auto iter = Net::LocalisedIterator(*this, centerFish);
    iter.gotoEnd();
    return iter;
}
Net::cell &Net::at(vec cord) {
    int x = std::floor(cord.x);
    int y = std::floor(cord.y);
    if (x < -1 || y < -1 || x > static_cast<int>(cellCnt) || y > static_cast<int>(cellCnt)) {
        throw std::out_of_range("Indexing out of grid!");
    }
    return grid[x + 1][y + 1];
}