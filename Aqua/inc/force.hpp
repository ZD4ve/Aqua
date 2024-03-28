#pragma once
#include <SFML/Graphics.hpp>

namespace aq {
class Fish;

class Force {
   protected:
    Fish *me;
    sf::Vector2f sum;
    float weight;

   public:
    Force();
    void setMe(Fish *me);
    sf::Vector2f getSum();
    virtual void accum(Fish &nb) = 0;
    virtual void finalize() = 0;
    virtual ~Force() = 0;
};

}  // namespace aq