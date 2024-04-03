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
    Fish() = default;
    Fish(sf::Vector2f pos, const std::vector<Force *> &forces, float vision);
    Fish(const Fish &rhs);
    Fish &operator=(const Fish &rhs);
    sf::Vector2f getLocation() const { return position; }
    bool canSee(Fish &near) const;
    void draw(sf::RenderTarget &target);
    template <typename iterator>
    void move(sf::Time deltaT, iterator begin, iterator end) {
        for (iterator it = begin; it != end; ++it) {
            Fish &near = *it;
            if (&near == this) continue;
            if (!canSee(near)) continue;
            for (auto &force : forces) {
                force->accum(near);
            }
        }
        for (auto &force : forces) {
            force->finalize();
            velocity += force->getSum() * static_cast<float>(deltaT.asMicroseconds() / 1000.0);
        }
        position += velocity;
    }
    ~Fish();
};
}  // namespace aq