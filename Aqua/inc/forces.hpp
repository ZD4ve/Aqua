#pragma once
#include <cmath>

#include "force.hpp"

namespace {
float lenght(sf::Vector2f v) {
    return hypotf(v.x, v.y);
}
sf::Vector2f normalize(sf::Vector2f v) {
    float len = lenght(v);
    if (len < 0.00001) return sf::Vector2f(sin(rand() / 1000.0), cos(rand() / 1000.0));
    return sf::Vector2f(v.x / len, v.y / len);
}
float square_signed(float x) {
    return fabs(x) * x;
}
}  // namespace

namespace aq {

class SeparationForce : public Force {
   protected:
    float safe_dist;

   public:
    explicit SeparationForce(float weight, float safe_distance) : Force(weight), safe_dist(safe_distance){};
    virtual void accum(Fish &near) {
        sf::Vector2f diff = near.getLocation() - me->getLocation();
        float dist = lenght(diff);
        if (dist < safe_dist) sum -= diff * (safe_dist - dist) * (safe_dist - dist);
    }
    virtual void finalize() {}
    virtual ~SeparationForce() {}
    virtual Force *clone() {
        Force *ptr = new SeparationForce{*this};
        ptr->setMe(nullptr);
        return ptr;
    }
};

class AlignmentForce : public Force {
   protected:
    size_t n_of_close{0};

   public:
    explicit AlignmentForce(float weight) : Force(weight){};
    virtual void accum(Fish &near) {
        sum += near.getVelocity();
        ++n_of_close;
    }
    virtual void finalize() {
        if (n_of_close == 0) return;
        sum.x /= n_of_close;
        sum.y /= n_of_close;
        sum -= me->getVelocity();
        n_of_close = 0;
    }
    virtual ~AlignmentForce() {}
    virtual Force *clone() {
        Force *ptr = new AlignmentForce{*this};
        ptr->setMe(nullptr);
        return ptr;
    }
};

class CohesionForce : public Force {
   protected:
    size_t n_of_close{0};

   public:
    explicit CohesionForce(float weight) : Force(weight){};
    virtual void accum(Fish &near) {
        sf::Vector2f diff = near.getLocation() - me->getLocation();
        sum += diff;
        ++n_of_close;
    }
    virtual void finalize() {
        if (n_of_close == 0) return;
        sum.x /= n_of_close;
        sum.y /= n_of_close;
        n_of_close = 0;
    }
    virtual ~CohesionForce() {}
    virtual Force *clone() {
        Force *ptr = new CohesionForce{*this};
        ptr->setMe(nullptr);
        return ptr;
    }
};

class WaterResistanteForce : public Force {
   protected:
   public:
    explicit WaterResistanteForce(float weight) : Force(weight){};
    virtual void accum(Fish &near) {
        (void)near;
    }
    virtual void finalize() {
        sf::Vector2f v = me->getVelocity();
        sum -= sf::Vector2f(square_signed(v.x), square_signed(v.y));
    }
    virtual ~WaterResistanteForce() {}
    virtual Force *clone() {
        Force *ptr = new WaterResistanteForce{*this};
        ptr->setMe(nullptr);
        return ptr;
    }
};

class MinSpeedForce : public Force {
   protected:
   public:
    explicit MinSpeedForce(float weight) : Force(weight){};
    virtual void accum(Fish &near) {
        (void)near;
    }
    virtual void finalize() {
        sf::Vector2f v = me->getVelocity();
        if (lenght(v) < weight) {
            sum += normalize(v);
        }
    }
    virtual ~MinSpeedForce() {}
    virtual Force *clone() {
        Force *ptr = new MinSpeedForce{*this};
        ptr->setMe(nullptr);
        return ptr;
    }
};

}  // namespace aq