#pragma once
#include <forward_list>
#include <mutex>
#include <vector>

#include "fish.hpp"

namespace aq {
class Net {
   public:
    typedef std::forward_list<Fish *> cell;
    class LocalisedIterator {
       private:
        const Net &net;
        const sf::Vector2u centerCord;
        cell::iterator currIter;
        cell::iterator currEnd;
        size_t idx;
        sf::Vector2u currCord() const {
            return sf::Vector2u(centerCord.x + (idx % 3), centerCord.y + (idx / 3));
        }
        void updateIters() {
            currIter = net.grid[currCord().x + 1][currCord().y + 1].begin();
            currEnd = net.grid[currCord().x + 1][currCord().y + 1].end();
        }
        bool atEnd() {
            return idx == 9;
        }

       public:
        LocalisedIterator(const Net &net, const Fish &centerFish) : net(net), centerCord(net.getCord(centerFish)), idx(0) {
            updateIters();
            while (currIter == currEnd) {
                idx++;
                updateIters();
            }
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
            while (currIter == currEnd) {
                idx++;
                if (atEnd()) return *this;
                updateIters();
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

    struct Settings {
        size_t n_of_fishes = 100;
        size_t n_of_species = 1;
        size_t randomness_pct = 0;
    };

   private:
    const Settings opt;
    Fish *storage;
    cell **grid;
    size_t mapSize;
    double cellSize;
    sf::Clock lastUpdate;
    std::mutex working;
    size_t cellCnt() { return mapSize / cellSize; }
    sf::Vector2u getCord(const Fish &fish) const;

    LocalisedIterator begin(const Fish &centerFish) const;
    const LocalisedIterator cend(const Fish &centerFish) const;

   public:
    explicit Net(Settings fishSettings, size_t mapSize = 1000);

    void draw(sf::RenderTarget &target);
    void moveFish();

    ~Net();
};

}  // namespace aq
