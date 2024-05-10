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
        vec mapSize;
    };
    struct Dependency {
        const Island::Map *map;
        const std::atomic<sf::Vector2f> *mousePos;
        bool isSet() const {
            return map != nullptr &&
                   mousePos != nullptr;
        }
    };

   private:
    const Settings opt;
    const Dependency dep;
    double max_vision = 0;

   public:
    explicit Breeder(Settings fishSettings, Dependency forceDependencies);
    size_t getCnt() const {
        return opt.n_of_fishes;
    }
    /**
     * @brief Returns the furthest distance a fish can see
     * @warning Only callable after fish generation!
     */
    double getMaxVision() const {
        if (max_vision == 0) throw std::logic_error("Fish hasn't been generated yet!");
        return max_vision;
    }
    /**
     * @brief Generates the fishes
     * @return an array of the generated fishes, deletion is the callers responsibility
     */
    Fish *make();
};
}  // namespace aq
