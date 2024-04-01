#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "force.hpp"

namespace aq {

class Fish {
   private:
    sf::Vector2f position;
    sf::Vector2f velocity;
    std::vector<Force *> forces;
    float vision;
    sf::Sprite sp;

   public:
    Fish &operator=(const Fish&) = delete;
    Fish(sf::Vector2f pos, const std::vector<Force *> &forces, float vision);
    Fish(const Fish& rhs);
    sf::Vector2f getLocation() const {return position;}
    void draw(sf::RenderTarget &target);
    void move(sf::Time dt);
    bool canSee(Fish &nb) const;
    ~Fish();

};
}  // namespace aq