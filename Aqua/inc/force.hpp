#pragma once

#include "vec.hpp"

namespace aq {
class Fish;

class Force {
   protected:
    Fish *me{nullptr};
    vec sum{0, 0};
    double weight;

   public:
    explicit Force(double weight) : weight(weight){};
    void setMe(Fish *me);
    vec getSum();
    virtual void accum(const Fish &near) = 0;
    virtual void finalize() = 0;
    virtual ~Force(){};
    virtual Force *clone() = 0;
};

}  // namespace aq