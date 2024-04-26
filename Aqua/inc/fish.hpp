#pragma once
#include <SFML/Graphics.hpp>
#include <atomic>
#include <vector>

#include "force.hpp"
#include "vec.hpp"

namespace aq {

class Fish {
   private:
    vec position;
    vec velocity;
    std::vector<Force *> forces;
    std::atomic_bool dead{false};

    static constexpr size_t n_of_animations = 4;
    static sf::Texture *tex;
    static size_t instance_cnt;
    void loadTexture();

    float vision;
    sf::Sprite sp;
    size_t animation_state{0};
    sf::Clock last_animation_update;

   public:
    Fish();
    Fish(vec pos, const std::vector<Force *> &forces, float vision, sf::Color color);
    Fish(const Fish &rhs);
    Fish &operator=(const Fish &rhs);
    vec getLocation() const { return position; }
    vec getVelocity() const { return velocity; }
    float getVision() const { return vision; }
    bool canSee(const vec &pos) const;
    bool canSee(Fish &near) const {
        return canSee(near.position);
    }
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
        vec delta = velocity * deltaT.asSeconds();
        if (delta.len() > vision) delta = delta.norm() * vision;
        position += velocity * deltaT.asSeconds();
    }
    ~Fish();
};
}  // namespace aq