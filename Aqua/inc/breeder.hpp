#pragma once
#include <SFML/Graphics.hpp>

#include "fish.hpp"

namespace aq {

class Breeder {
   public:
    struct Settings {
        size_t n_of_fishes = 100;
        size_t n_of_species = 1;
        size_t randomness_pct = 0;
    };

   private:
    const Settings opt;
    double max_vision = 0;

   public:
    Breeder(Settings fishSettings);
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
