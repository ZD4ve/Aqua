#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

namespace aq {
class Force;

class Fish {
   private:
    sf::Vector2f position;
    sf::Vector2f velocity;
    std::vector<Force *> forces;
    sf::Sprite sp;
    float vision;

   public:
    Fish();
    void draw(sf::RenderTarget &target);
    void move(sf::Time dt);
    bool canSee(Fish &nb);
    ~Fish();
};
}  // namespace aq