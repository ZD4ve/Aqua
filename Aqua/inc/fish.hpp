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
    size_t species_id;
    std::atomic_bool dead{false};

    static constexpr size_t n_of_animations = 4;
    static sf::Texture *tex;
    static size_t instance_cnt;
    static void loadTexture();
    static bool GUIactive() {
        return sf::Context::getActiveContextId() != 0;
    }

    double vision;
    sf::Sprite sp;
    size_t animation_state{0};
    sf::Clock last_animation_update;

   public:
    Fish();
    Fish(vec pos, const std::vector<Force *> &forces, double vision, sf::Color color, size_t species);
    Fish(const Fish &rhs);
    Fish &operator=(const Fish &rhs);
    vec getLocation() const { return position; }
    vec getVelocity() const { return velocity; }
    double getVision() const { return vision; }
    bool canSee(const vec &pos) const;
    void kill() {
        dead.store(true);
        if (GUIactive()) {
            sp.setColor(sf::Color(242, 235, 227));
            sp.setTexture(tex[n_of_animations]);
        }
    }
    bool canSee(const Fish &near) const {
        return canSee(near.position);
    }
    bool sameSpeciesAs(const Fish &near) const {
        return species_id == near.species_id;
    }
    void draw(sf::RenderTarget &target);
    template <typename iterator>
    void move(sf::Time deltaT, iterator begin, iterator end) {
        if (dead) return;
        for (iterator it = begin; it != end; ++it) {
            Fish &near = *it;
            if (&near == this) continue;
            if (!canSee(near)) continue;
            if (near.dead) continue;
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