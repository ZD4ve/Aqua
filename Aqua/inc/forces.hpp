#pragma once
#include <cmath>

#include "force.hpp"
#include "island.hpp"

namespace {
inline float lenght(sf::Vector2f v) {
    return std::hypot(v.x, v.y);
}
inline sf::Vector2f normalize(sf::Vector2f v) {
    float len = lenght(v);
    if (len < 0.00001) return sf::Vector2f(std::sin(std::rand() / 1000.0), std::cos(std::rand() / 1000.0));
    return sf::Vector2f(v.x / len, v.y / len);
}
inline float square(float x) {
    return x * x;
}
inline float square_signed(float x) {
    return std::abs(x) * x;
}
}  // namespace

namespace aq {

class SeparationForce : public Force {
   protected:
    const float safeDist;

   public:
    explicit SeparationForce(float weight, float safe_distance) : Force(weight), safeDist(safe_distance){};
    virtual void accum(Fish &near) {
        sf::Vector2f diff = near.getLocation() - me->getLocation();
        float dist = lenght(diff);
        if (dist < safeDist) sum -= diff * square(safeDist - dist);
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

class MouseForce : public Force {
   protected:
    const float fearDist;
    const std::atomic<sf::Vector2f> &mousePosition;

   public:
    explicit MouseForce(float weight, float fear_distance, const std::atomic<sf::Vector2f> &mouse_position) : Force(weight), fearDist(fear_distance), mousePosition(mouse_position){};
    virtual void accum(Fish &near) {
        (void)near;
    }
    virtual void finalize() {
        sf::Vector2f mouse = mousePosition.load();
        if (mouse == sf::Vector2f(0, 0)) return;
        sf::Vector2f diff = mouse - me->getLocation();
        float dist = lenght(diff);
        if (dist < fearDist) sum -= diff * (fearDist - dist);
    }
    virtual Force *clone() {
        Force *ptr = new MouseForce{*this};
        ptr->setMe(nullptr);
        return ptr;
    }
};

class IslandForce : public Force {
   protected:
    const Island::Map &map;

    inline constexpr static const size_t nOfSamplePoints = 36;
    inline constexpr static const struct {
        float x, y;
    } samplePoints[nOfSamplePoints] =
        {{1.000, 0.000}, {0.940, 0.342}, {0.766, 0.643}, {0.500, 0.866}, {0.174, 0.985}, {-0.174, 0.985}, {-0.500, 0.866}, {-0.766, 0.643}, {-0.940, 0.342}, {-1.000, 0.000}, {-0.940, -0.342}, {-0.766, -0.643}, {-0.500, -0.866}, {-0.174, -0.985}, {0.174, -0.985}, {0.500, -0.866}, {0.766, -0.643}, {0.940, -0.342}, {0.667, 0.000}, {0.577, 0.333}, {0.333, 0.577}, {0.000, 0.667}, {-0.333, 0.577}, {-0.577, 0.333}, {-0.667, 0.000}, {-0.577, -0.333}, {-0.333, -0.577}, {-0.000, -0.667}, {0.333, -0.577}, {0.577, -0.333}, {0.333, 0.000}, {0.167, 0.289}, {-0.167, 0.289}, {-0.333, 0.000}, {-0.167, -0.289}, {0.167, -0.289}};

   public:
    explicit IslandForce(float weight, const Island::Map &map) : Force(weight), map(map){};
    virtual void accum(Fish &near) {
        (void)near;
    }
    virtual void finalize() {
        float vis = me->getVision();
        sf::Vector2f loc = me->getLocation();
        for (size_t i = 0; i < nOfSamplePoints; i++) {
            sf::Vector2f sample(samplePoints[i].x, samplePoints[i].y);
            sf::Vector2f offset = sample * vis;
            if (map(loc + offset)) continue;
            sum -= offset / square(lenght(sample));
        }
    }
    virtual Force *clone() {
        Force *ptr = new IslandForce{*this};
        ptr->setMe(nullptr);
        return ptr;
    }
};

}  // namespace aq