#pragma once
#include <SFML/Graphics.hpp>
#include <atomic>
#include <vector>

#include "force.hpp"
#include "vec.hpp"

namespace aq {

/**
 * @brief Represents a fish
 * @details
 * It has a position, a velocity and stores the forces acting on it.
 * It staticly stores some textures and can be drawn to the screen.
 */
class Fish {
   private:
    vec position;
    vec velocity;
    std::vector<Force *> forces;
    size_t species_id;
    std::atomic_bool dead{false};

    static constexpr size_t n_of_animations = 4;
    static sf::Texture *tex;
    static size_t instance_cnt;  ///< Number of instances for texture deletion
    /**
     * @brief Loads the textures
     * @details Only loads them if they haven't been loaded yet and if there is a GUI
     */
    static void loadTexture();
    static bool GUIactive() {
        static bool active = sf::Context::getActiveContextId() != 0;
        return active;
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
    bool canSee(const Fish &near) const {
        return canSee(near.position);
    }
    bool sameSpeciesAs(const Fish &near) const {
        return species_id == near.species_id;
    }
    /**
     * @brief Kills the fish
     * @details Changes the texture to a skeleton, it will no longer move or effect other fish
     */
    void kill() {
        dead.store(true);
        if (GUIactive()) {
            sp.setColor(sf::Color(242, 235, 227));
            sp.setTexture(tex[n_of_animations]);
        }
    }
    void draw(sf::RenderTarget &target);
    /**
     * @brief Moves the fish according to it's internal forces
     * @tparam iterator for a container of fish that effect *this
     * @param deltaT time passed since last move call
     */
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
            velocity += force->getValue() * deltaT.asSeconds();
        }
        vec delta = velocity * deltaT.asSeconds();
        if (delta.len() > vision) delta = delta.norm() * vision;
        position += velocity * deltaT.asSeconds();
    }
    ~Fish();
};
}  // namespace aq