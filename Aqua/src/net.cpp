#include "net.hpp"

#include <cmath>

using namespace aq;
using namespace std::chrono;

Net::Net(Breeder breeder, size_t mapSize) : fish_cnt(breeder.getCnt()), mapSize(mapSize) {
    storage = breeder.make();
    size_t cnt = std::floor(mapSize / breeder.getMaxVision());
    if (cnt == 0) throw std::logic_error("Fish see farther than map size!");
    cellSize = mapSize / static_cast<float>(cnt);
    cellCnt = mapSize / cellSize;

    grid = new Net::cell *[cellCnt + 2];
    grid[0] = new Net::cell[(cellCnt + 2) * (cellCnt + 2)];
    for (size_t i = 1; i < cellCnt + 2; i++) {
        grid[i] = grid[i - 1] + cellCnt + 2;
    }

    for (size_t i = 0; i < fish_cnt; i++) {
        at(getCord(storage[i])).push_front(&storage[i]);
    }
}

void Net::moveFishWhile(std::stop_token life) {
    sf::Time deltaT;
    while (!life.stop_requested()) {
        deltaT = lastUpdate.restart();
        // separated to 2 loops for the possibility of parallelism
        for (size_t j = 0; j < 9; j++) {
            for (size_t i = j; i < cellCnt * cellCnt; i += 9) {
                vec cord(i % cellCnt, i / cellCnt);

                cell &current_cell = at(cord);
                std::lock_guard lock{working};
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
        std::this_thread::sleep_for(1ms);
    }
}
Net::cell &Net::at(vec cord) {
    auto v = cord.whole();
    v.x = std::clamp(v.x, -1L, static_cast<ssize_t>(cellCnt));
    v.y = std::clamp(v.y, -1L, static_cast<ssize_t>(cellCnt));

    return grid[v.x][v.y];
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
