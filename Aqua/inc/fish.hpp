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

    static sf::Texture *tex;
    static size_t instance_cnt;

    float vision;
    sf::Sprite sp;

   public:
    Fish();
    Fish(sf::Vector2f pos, const std::vector<Force *> &forces, float vision, sf::Color color);
    Fish(const Fish &rhs);
    Fish &operator=(const Fish &rhs);
    sf::Vector2f getLocation() const { return position; }
    sf::Vector2f getVelocity() const { return velocity; }
    bool canSee(const sf::Vector2f &pos) const;
    bool canSee(Fish &near) const {
        return canSee(near.position);
    }
    float getVision() { return vision; }
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
            velocity += force->getSum() * deltaT.asSeconds();
        }
        position += velocity * deltaT.asSeconds();
    }
    ~Fish();
};
}  // namespace aq