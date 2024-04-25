#include "net.hpp"

#include <cmath>

#ifdef PARAL
#include <omp.h>
#endif

using namespace aq;
using namespace std::chrono;

Net::Net(Breeder breeder, size_t mapSize) : fish_cnt(breeder.getCnt()), mapSize(mapSize) {
    storage = breeder.make();
    size_t cnt = std::floor(mapSize / breeder.getMaxVision());
    if (cnt == 0) throw std::logic_error("Fish see farther than map size!");
    cellSize = mapSize / static_cast<float>(cnt);
    cellCnt = mapSize / cellSize;
    grid = new Net::cell *[cellCnt + 4];
    grid[0] = new Net::cell[(cellCnt + 4) * (cellCnt + 4)];
    for (size_t i = 1; i < cellCnt + 4; i++) {
        grid[i] = grid[i - 1] + cellCnt + 4;
    }
    for (size_t i = 0; i < fish_cnt; i++) {
        at(getCord(storage[i])).push_back(&storage[i]);
    }
}

void Net::moveFish() {
    working.lock();
    sf::Time deltaT = lastUpdate.restart();

#ifdef PARAL
    for (size_t j = 0; j < 9; j++) {
#pragma omp parallel
#pragma omp for
        for (size_t i = j; i < cellCnt * cellCnt; i += 9) {
            vec cord(i % cellCnt, i / cellCnt);

            cell current_cell = at(cord);
            for (cell::iterator iter = current_cell.begin(); iter != current_cell.end();) {
                Fish &fish = **iter;
                /*vec loc = fish.getLocation();
                if (loc.x < 0 || loc.y < 0 || loc.x > mapSize || loc.y > mapSize) {
                    ++iter;
                    continue;
                }
                */

                /*vec pos = getCord(fish);
                printf("%lf %lf %lf %lf\n", pos.x, pos.y, cord.x, cord.y);
                if (!cord.wholeEQ(pos))
                    throw std::runtime_error("Fish is not in its cell!");*/

                fish.move(deltaT, begin(fish), end(fish));

                vec new_cord = getCord(fish);
                if (!cord.wholeEQ(new_cord)) {
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
    /*
    for (size_t i = 0; i < fish_cnt; i++) {
        storage[i].move(deltaT, storage, storage + fish_cnt);
    }
    */
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
    auto v = cord.whole();
    if (v.x < -2 || v.y < -2 || v.x > static_cast<ssize_t>(cellCnt + 1) || v.y > static_cast<ssize_t>(cellCnt + 1)) {
        throw std::out_of_range("Indexing out of grid!");
    }
    return grid[v.x + 2][v.y + 2];
}