#pragma once

#include "vec.hpp"

namespace aq {
class Fish;

class Force {
   protected:
    const Fish *me{nullptr};
    vec sum{0, 0};
    float weight;

   public:
    explicit Force(float weight) : weight(weight){};
    void setMe(Fish *me);
    vec getSum();
    virtual void accum(const Fish &near) = 0;
    virtual void finalize() = 0;
    virtual ~Force(){};
    virtual Force *clone() = 0;
};

}  // namespace aq