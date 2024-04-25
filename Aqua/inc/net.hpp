#pragma once
#include <list>
#include <mutex>
#include <vector>

#include "breeder.hpp"
#include "fish.hpp"

namespace aq {
class Net {
   public:
    typedef std::list<Fish *> cell;
    class LocalisedIterator {
       private:
        const Net &net;
        const sf::Vector2u centerCord;
        cell::iterator currIter;
        cell::iterator currEnd;
        size_t idx{0};
        sf::Vector2u currCord() const {
            return sf::Vector2u(centerCord.x + (idx % 3) - 1, centerCord.y + (idx / 3) - 1);
        }
        void updateIters() {
            currIter = net.grid[currCord().x + 1][currCord().y + 1].begin();
            currEnd = net.grid[currCord().x + 1][currCord().y + 1].end();
        }
        bool atEnd() const {
            return idx == 9;
        }

       public:
        LocalisedIterator(const Net &net, const Fish &centerFish);
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
        LocalisedIterator &operator++();
        LocalisedIterator operator++(int) {
            auto tmp = *this;
            operator++();
            return tmp;
        }
        bool operator!=(const LocalisedIterator &rhs) {
            return currIter != rhs.currIter;
        }
    };

   private:
    const size_t fish_cnt;
    Fish *storage;
    cell **grid;
    const size_t mapSize;
    double cellSize;
    size_t cellCnt;
    sf::Clock lastUpdate;
    std::mutex working;
    sf::Vector2u getCord(const Fish &fish) const;

    LocalisedIterator begin(const Fish &centerFish) const;
    LocalisedIterator end(const Fish &centerFish) const;

   public:
    explicit Net(Breeder breeder, size_t mapSize = 1000);

    void draw(sf::RenderTarget &target);
    void moveFish();

    ~Net();
};

}  // namespace aq
