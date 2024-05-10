#pragma once

#include "vec.hpp"

namespace aq {
class Fish;

/**
 * @brief A force that can be applied to a fish
 * @details
   Order of operations:
    1. accum\n
    2. finalize\n
    3. getValue\n
 */
class Force {
   protected:
    Fish *me{nullptr};
    vec sum{0, 0};
    double weight;

   public:
    explicit Force(double weight) : weight(weight){};
    /**
     * @brief Sets the fish that is containing this force
     * @warning Must be set before using the force
     */
    void setMe(Fish *me);
    /**
     * @brief Should be called for each fish in the vicinity
     */
    virtual void accum(const Fish &near) = 0;
    /**
     * @brief After accumulation finalize the calculation
     */
    virtual void finalize() = 0;
    /**
     * @brief Returns the calculated value of the force and resets it
     */
    vec getValue();
    virtual ~Force() = default;
    /**
     * @brief Clones the force
     * @return A dynamically allocated copy of the force, with the me pointer reset
     */
    virtual Force *clone() = 0;
};

}  // namespace aq