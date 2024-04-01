#pragma once
#include <forward_list>
#include <vector>

#include "fish.hpp"

namespace aq {
class Net {
   public:
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
        std::forward_list<Fish *>::iterator currIter;
        std::forward_list<Fish *>::iterator currEnd;

        size_t idx;
        sf::Vector2u currCord() const {
            return sf::Vector2u(centerCord.x + (idx % 3), centerCord.y + (idx / 3));
        }

       public:
        LocalisedIterator(const Net &net, const Fish &centerFish)  : net(net), centerFish(centerFish), centerCord(net.getCord(centerFish)){
            idx = (centerCord.x == 0 ? 1 : 0) + (centerCord.y == 0 ? 3 : 0);
            auto &cell = net.grid[currCord().x][currCord().y];
            currIter = cell.begin();
            currEnd = cell.end();
            while (currIter==currEnd || *currIter == &centerFish)
            {
                idx++;
                if(atEnd()) return;
                currIter = net.grid[currCord().x][currCord().y].begin();
            }
        }
        bool atEnd(){
            return idx == 9;
        }
        Fish &operator*() {
            if(idx == 9) throw std::out_of_range("Iter already at end!");
            return **currIter;
        }
        Fish *operator->() {
            if(atEnd()) throw std::out_of_range("Iter already at end!");
            return *currIter;
        }
        LocalisedIterator &operator++() {
            if(atEnd()) throw std::out_of_range("Iter already at end!");
            currIter++;
            if(*currIter == &centerFish) currIter++;
            while (currIter==currEnd)
            {
                idx++;
                if(atEnd()) return *this;
                currIter = net.grid[currCord().x][currCord().y].begin();
                if(*currIter == &centerFish) currIter++;
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
    std::vector<Fish> storage;

    std::forward_list<Fish *> **grid;
    size_t mapSize;
    size_t cellCnt() { return mapSize / cellSize; }
    size_t cellSize = 1;
    sf::Vector2u getCord(const Fish &fish) const;

   public:
    Net(Settings fishSettings, size_t approximateMapSize = 1000);
    LocalisedIterator begin(const Fish &centerFish) const;
    ~Net();
};

}  // namespace aq
