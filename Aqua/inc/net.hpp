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
        Net &net;
        const sf::Vector2i centerCord;
        cell::iterator currIter;
        cell::iterator currEnd;
        size_t idx{0};
        sf::Vector2i currCord() const {
            return sf::Vector2i(centerCord.x + (idx % 3) - 1, centerCord.y + (idx / 3) - 1);
        }
        void updateIters() {
            currIter = net.at(currCord()).begin();
            currEnd = net.at(currCord()).end();
        }
        bool atEnd() const {
            return idx == 9;
        }

       public:
        LocalisedIterator(Net &net, const Fish &centerFish);
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
    sf::Vector2i getCord(const Fish &fish) const;
    cell &at(sf::Vector2i cord);

    LocalisedIterator begin(const Fish &centerFish);
    LocalisedIterator end(const Fish &centerFish);

   public:
    explicit Net(Breeder breeder, size_t mapSize = 1000);

    void draw(sf::RenderTarget &target);
    void moveFish();

    ~Net();
};

}  // namespace aq
