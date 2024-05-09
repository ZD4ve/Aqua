#include "net.hpp"

// #include <omp.h>

#include <atomic>
#include <cmath>
#include <thread>

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
        at(getCord(storage[i])).push_front(&storage[i]);
    }
}

void Net::moveFishWhile(std::atomic_bool &live) {
    sf::Time deltaT;
    // #pragma omp parallel shared(deltaT)
    while (live) {
        // #pragma omp master
        working.lock();
        // #pragma omp single
        deltaT = lastUpdate.restart();
        for (size_t j = 0; j < 9; j++) {
            // #pragma omp for schedule(dynamic)
            for (size_t i = j; i < cellCnt * cellCnt; i += 9) {
                vec cord(i % cellCnt, i / cellCnt);

                cell &current_cell = at(cord);
                for (cell::iterator iter = current_cell.begin(); iter != current_cell.end();) {
                    Fish &fish = **iter;

                    fish.move(deltaT, begin(fish), end(fish));

                    vec new_cord = getCord(fish);
                    if (!cord.wholeEQ(new_cord)) {
                        at(new_cord).push_front(*iter);
                        iter = current_cell.erase(iter);
                    } else {
                        ++iter;
                    }
                }
            }
        }
        // #pragma omp master
        working.unlock();
        std::this_thread::sleep_for(1ns);
    }
}
Net::cell &Net::at(vec cord) {
    auto v = cord.whole();
    if (v.x < -2 || v.y < -2 || v.x > static_cast<ssize_t>(cellCnt + 1) || v.y > static_cast<ssize_t>(cellCnt + 1)) {
        throw std::out_of_range("Indexing out of grid!");
    }
    return grid[v.x + 2][v.y + 2];
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

Net::LocalizedIterator Net::begin(const Fish &centerFish) {
    return Net::LocalizedIterator(*this, centerFish);
}
Net::LocalizedIterator Net::end(const Fish &centerFish) {
    auto iter = Net::LocalizedIterator(*this, centerFish);
    iter.gotoEnd();
    return iter;
}
