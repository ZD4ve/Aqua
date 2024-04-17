#pragma once
#include <SFML/Graphics.hpp>

namespace aq {
class Fish;

class Force {
   protected:
    Fish *me{nullptr};
    sf::Vector2f sum{0, 0};
    float weight;

   public:
    explicit Force(float weight) : weight(weight){};
    void setMe(Fish *me);
    sf::Vector2f getSum();
    virtual void accum(Fish &near) = 0;
    virtual void finalize() = 0;
    virtual ~Force(){};
    virtual Force *clone() = 0;
};

}  // namespace aq