#pragma once
#include <SFML/Graphics.hpp>

namespace aq {
class Fish;

class Force {
   protected:
    Fish *me{nullptr};
    sf::Vector2f sum;
    float weight{0};

   public:
    void setMe(Fish *me);
    sf::Vector2f getSum();
    virtual void accum(Fish &near) = 0;
    virtual void finalize() = 0;
    virtual ~Force() = 0;
    virtual Force *clone() = 0;
};

}  // namespace aq