#pragma once
#include <list>
#include <mutex>
#include <stop_token>
#include <vector>

#include "breeder.hpp"
#include "fish.hpp"
#include "vec.hpp"

namespace aq {

/**
 * @brief The net stores the fish and provides a cell based LUT
 */
class Net {
   public:
    using cell = std::list<Fish *>;
    /**
     * @brief Iterates over the cells in the visual range of a fish
     */
    class LocalizedIterator {
       private:
        Net &net;
        const vec centerCord;
        cell::iterator currIter;
        cell::iterator currEnd;
        size_t idx{0};
        vec currCord() const {
            return vec(centerCord.x + (idx % 3) - 1, centerCord.y + (idx / 3) - 1);
        }
        void updateIters() {
            currIter = net.at(currCord()).begin();
            currEnd = net.at(currCord()).end();
        }

       public:
        LocalizedIterator(Net &net, const Fish &centerFish);
        void gotoEnd() {
            idx = 8;
            updateIters();
            currIter = currEnd;
        }
        Fish &operator*() {
            return **currIter;
        }
        Fish *operator->() {
            return *currIter;
        }
        LocalizedIterator &operator++();
        LocalizedIterator operator++(int) {
            auto tmp = *this;
            operator++();
            return tmp;
        }
        bool operator!=(const LocalizedIterator &rhs) {
            return currIter != rhs.currIter;
        }
    };

   private:
    const size_t fish_cnt;
    Fish *storage;
    const size_t mapSize;
    sf::Clock lastUpdate;
    std::mutex working;  //< Blocks drawing while moving fish and vice versa
    cell **grid;         //< location based lookup table
    double cellSize;
    size_t cellCnt;
    vec getCord(const Fish &fish) const;
    cell &at(vec cord);

    LocalizedIterator begin(const Fish &centerFish);
    LocalizedIterator end(const Fish &centerFish);
    Fish *begin() { return storage; }
    Fish *end() { return storage + fish_cnt; }

   public:
    explicit Net(Breeder breeder, size_t mapSize = 1000);

    void draw(sf::RenderTarget &target);

    /**
     * @brief Infinitely loop that moves the fish until another thread sets live to false
     * @returns after live is set to false and the last iteration is finished
     */
    void moveFishWhile(std::stop_token live);

    ~Net();
};

}  // namespace aq
