#pragma once
#include <atomic>

#include "fish.hpp"
#include "island.hpp"
#include "vec.hpp"

namespace aq {

class Breeder {
   public:
    struct Settings {
        size_t n_of_fishes = 100;
        size_t n_of_species = 1;
        size_t randomness_pct = 0;
    };
    struct Dependency {
        const Island::Map *map;
        const std::atomic<sf::Vector2f> *mousPos;
        bool isSet() const {
            return map != nullptr &&
                   mousPos != nullptr;
        }
    };

   private:
    const Settings opt;
    const Dependency dep;
    double max_vision = 0;

   public:
    explicit Breeder(Settings fishSettings, Dependency forceDependecies);
    size_t getCnt() const {
        return opt.n_of_fishes;
    }
    double getMaxVision() const {
        if (max_vision == 0) throw std::logic_error("Fish hasnt been generated yet!");
        return max_vision;
    }
    Fish *make();
};
}  // namespace aq
