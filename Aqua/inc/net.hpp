#pragma once
#include <forward_list>
#include <vector>

#include "fish.hpp"

namespace aq {
class Net {
   public:
    typedef std::forward_list<Fish *> cell;
    struct Settings {
        size_t n_of_fishes;
        size_t n_of_species;
        size_t srandomness_pct;
    };
    class LocalisedIterator {
       private:
        const Net &net;
        const Fish &centerFish;
        const sf::Vector2u centerCord;
        cell::iterator currIter;
        cell::iterator currEnd;
        size_t idx;
        sf::Vector2u currCord() const {
            return sf::Vector2u(centerCord.x + (idx % 3), centerCord.y + (idx / 3));
        }
        void updateIters() {
            currIter = net.grid[currCord().x+1][currCord().y+1].begin();
            currEnd = net.grid[currCord().x+1][currCord().y+1].end();
        }

       public:
        LocalisedIterator(const Net &net, const Fish &centerFish) : net(net), centerFish(centerFish), centerCord(net.getCord(centerFish)) {
            idx = 0;
            updateIters();
            while (currIter == currEnd || *currIter == &centerFish) {
                idx++;
                if (atEnd()) return;
                updateIters();
            }
        }
        bool atEnd() {
            return idx == 9;
        }
        void gotoEnd() {
            idx = 8;
            updateIters();
            currIter = currEnd;
            idx++;
        }
        Fish &operator*() {
            if (idx == 9) throw std::out_of_range("Iter already at end!");
            return **currIter;
        }
        Fish *operator->() {
            if (atEnd()) throw std::out_of_range("Iter already at end!");
            return *currIter;
        }
        LocalisedIterator &operator++() {
            if (atEnd()) throw std::out_of_range("Iter already at end!");
            currIter++;
            if (*currIter == &centerFish) currIter++;
            while (currIter == currEnd) {
                idx++;
                if (atEnd()) return *this;
                updateIters();
                if (*currIter == &centerFish) currIter++;
            }
            return *this;
        }
        LocalisedIterator operator++(int) {
            auto tmp = *this;
            operator++();
            return tmp;
        }
        bool operator==(const LocalisedIterator &rhs) {
            return currIter == rhs.currIter;
        }
    };

   private:
    const Settings opt;
    Fish *storage;
    cell **grid;
    size_t mapSize;
    size_t cellCnt() { return mapSize / cellSize; }
    size_t cellSize = 1;
    sf::Vector2u getCord(const Fish &fish) const;

   public:
    Net(Settings fishSettings, size_t approximateMapSize = 1000);
    LocalisedIterator begin(const Fish &centerFish) const;
    const LocalisedIterator cend(const Fish &centerFish) const;
    ~Net();
};

}  // namespace aq
