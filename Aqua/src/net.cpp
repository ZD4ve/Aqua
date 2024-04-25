#include "net.hpp"

#include <SFML/Graphics.hpp>
#include <cmath>

using namespace aq;
using namespace std::chrono;

Net::Net(Breeder breeder, size_t mapSize) : fish_cnt(breeder.getCnt()), mapSize(mapSize) {
    storage = breeder.make();
    size_t cnt = std::floor(mapSize / breeder.getMaxVision());
    if (cnt == 0) throw std::logic_error("Fish see farther than map size!");
    cellSize = mapSize / static_cast<double>(cnt);
    cellCnt = mapSize / cellSize;
    grid = new Net::cell *[cellCnt + 2];
    grid[0] = new Net::cell[(cellCnt + 2) * (cellCnt + 2)];
    for (size_t i = 1; i < cellCnt + 2; i++) {
        grid[i] = grid[i - 1] + cellCnt + 2;
    }
    for (size_t i = 0; i < fish_cnt; i++) {
        Fish &fish = storage[i];
        sf::Vector2u pos = getCord(fish);
        grid[pos.x][pos.y].emplace_front(&fish);
    }
}

Net::~Net() {
    delete[] storage;
    delete[] grid[0];
    delete[] grid;
}

sf::Vector2u Net::getCord(const Fish &fish) const {
    sf::Vector2f loc = fish.getLocation();
    return sf::Vector2u(loc.x / cellSize + 1, loc.y / cellSize + 1);
}

Net::LocalisedIterator Net::begin(const Fish &centerFish) const {
    return Net::LocalisedIterator(*this, centerFish);
}
Net::LocalisedIterator Net::end(const Fish &centerFish) const {
    auto iter = Net::LocalisedIterator(*this, centerFish);
    iter.gotoEnd();
    return iter;
}

void Net::draw(sf::RenderTarget &target) {
    working.lock();
    for (size_t i = 0; i < fish_cnt; i++) {
        storage[i].draw(target);
    }
    working.unlock();
}

void Net::moveFish() {
    working.lock();
    sf::Time deltaT = lastUpdate.restart();
    // TODO: paralell with localIter
    /*
    // #pragma omp parallel
    for (size_t j = 0; j < 9; j++) {
        // #pragma omp for
        for (size_t i = j; i < cellCnt * cellCnt; i += 9) {
            sf::Vector2u cord(i % cellCnt + 1, i / cellCnt + 1);

            cell current_cell = grid[cord.x][cord.y];
            for (auto iter = current_cell.begin(); iter != current_cell.end(); iter++) {
                printf("(%u, %u)\n", cord.x, cord.y);
                Fish &fish = **iter;
                fish.move(deltaT, begin(fish), end(fish));
                sf::Vector2u new_cord = getCord(fish);
                if (new_cord != cord) {
                    grid[new_cord.x][new_cord.y].push_back(*iter);
                    current_cell.erase(iter);
                }
            }
        }
    }
    */

    for (size_t i = 0; i < fish_cnt; i++) {
        storage[i].move(deltaT, storage, storage + fish_cnt);
    }
    working.unlock();
}
