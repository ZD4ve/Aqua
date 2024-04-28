#pragma once
#include <list>
#include <mutex>
#include <vector>

#include "breeder.hpp"
#include "fish.hpp"
#include "vec.hpp"

namespace aq {
class Net {
   public:
    typedef std::list<Fish *> cell;
    class LocalisedIterator {
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
        LocalisedIterator(Net &net, const Fish &centerFish);
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
    const size_t mapSize;
    sf::Clock lastUpdate;
    std::mutex working;
    cell **grid;
    double cellSize;
    size_t cellCnt;
    vec getCord(const Fish &fish) const;
    cell &at(vec cord);

    LocalisedIterator begin(const Fish &centerFish);
    LocalisedIterator end(const Fish &centerFish);

   public:
    explicit Net(Breeder breeder, size_t mapSize = 1000);

    void draw(sf::RenderTarget &target);
    void moveFishWhile(std::atomic_bool &live);

    ~Net();
};

}  // namespace aq
