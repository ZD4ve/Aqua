#pragma once
#include <cmath>

#include "force.hpp"
#include "island.hpp"
#include "vec.hpp"

namespace {
inline double square(double x) {
    return x * x;
}
inline double square_signed(double x) {
    return std::abs(x) * x;
}
}  // namespace

namespace aq {

class SeparationForce : public Force {
   protected:
    const double safeDist;

   public:
    explicit SeparationForce(double weight, double safe_distance) : Force(weight), safeDist(safe_distance){};
    virtual void accum(const Fish &near) {
        vec diff = near.getLocation() - me->getLocation();
        double dist = diff.len();
        if (dist < safeDist) sum -= diff * square(safeDist - dist);
    }
    virtual void finalize() {}
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
    explicit AlignmentForce(double weight) : Force(weight){};
    virtual void accum(const Fish &near) {
        sum += near.getVelocity();
        ++n_of_close;
    }
    virtual void finalize() {
        if (n_of_close == 0) return;
        sum.x /= static_cast<double>(n_of_close);
        sum.y /= static_cast<double>(n_of_close);
        sum -= me->getVelocity();
        n_of_close = 0;
    }
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
    explicit CohesionForce(double weight) : Force(weight){};
    virtual void accum(const Fish &near) {
        vec diff = near.getLocation() - me->getLocation();
        sum += diff;
        ++n_of_close;
    }
    virtual void finalize() {
        if (n_of_close == 0) return;
        sum.x /= static_cast<double>(n_of_close);
        sum.y /= static_cast<double>(n_of_close);
        n_of_close = 0;
    }
    virtual Force *clone() {
        Force *ptr = new CohesionForce{*this};
        ptr->setMe(nullptr);
        return ptr;
    }
};

class SpeciesCohesionForce : public Force {
   protected:
    size_t n_of_close{0};

   public:
    explicit SpeciesCohesionForce(double weight) : Force(weight){};
    virtual void accum(const Fish &near) {
        if (!me->sameSpeciesAs(near)) return;
        vec diff = near.getLocation() - me->getLocation();
        sum += diff;
        ++n_of_close;
    }
    virtual void finalize() {
        if (n_of_close == 0) return;
        sum.x /= static_cast<double>(n_of_close);
        sum.y /= static_cast<double>(n_of_close);
        n_of_close = 0;
    }
    virtual Force *clone() {
        Force *ptr = new SpeciesCohesionForce{*this};
        ptr->setMe(nullptr);
        return ptr;
    }
};

class WaterResistanceForce : public Force {
   public:
    explicit WaterResistanceForce(double weight) : Force(weight){};
    virtual void accum(const Fish &near) {
        (void)near;
    }
    virtual void finalize() {
        vec v = me->getVelocity();
        sum -= vec(square_signed(v.x), square_signed(v.y));
    }
    virtual Force *clone() {
        Force *ptr = new WaterResistanceForce{*this};
        ptr->setMe(nullptr);
        return ptr;
    }
};

class MinSpeedForce : public Force {
   public:
    explicit MinSpeedForce(double weight) : Force(weight){};
    virtual void accum(const Fish &near) {
        (void)near;
    }
    virtual void finalize() {
        vec v = me->getVelocity();
        if (v.len() < weight) {
            sum += v.norm();
        }
    }
    virtual Force *clone() {
        Force *ptr = new MinSpeedForce{*this};
        ptr->setMe(nullptr);
        return ptr;
    }
};

class MouseForce : public Force {
   protected:
    const double fearDist;
    const std::atomic<sf::Vector2f> &mousePosition;

   public:
    explicit MouseForce(double weight, double fear_distance, const std::atomic<sf::Vector2f> &mouse_position) : Force(weight), fearDist(fear_distance), mousePosition(mouse_position){};
    virtual void accum(const Fish &near) {
        (void)near;
    }
    virtual void finalize() {
        vec mouse = mousePosition.load();
        if (mouse == vec(0, 0)) return;
        vec diff = mouse - me->getLocation();
        double dist = diff.len();
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

    constexpr static const size_t nOfSamplePoints = 36;
    constexpr static const struct {
        double x, y;
    } samplePoints[nOfSamplePoints] =
        {{1.000, 0.000}, {0.940, 0.342}, {0.766, 0.643}, {0.500, 0.866}, {0.174, 0.985}, {-0.174, 0.985}, {-0.500, 0.866}, {-0.766, 0.643}, {-0.940, 0.342}, {-1.000, 0.000}, {-0.940, -0.342}, {-0.766, -0.643}, {-0.500, -0.866}, {-0.174, -0.985}, {0.174, -0.985}, {0.500, -0.866}, {0.766, -0.643}, {0.940, -0.342}, {0.667, 0.000}, {0.577, 0.333}, {0.333, 0.577}, {0.000, 0.667}, {-0.333, 0.577}, {-0.577, 0.333}, {-0.667, 0.000}, {-0.577, -0.333}, {-0.333, -0.577}, {-0.000, -0.667}, {0.333, -0.577}, {0.577, -0.333}, {0.333, 0.000}, {0.167, 0.289}, {-0.167, 0.289}, {-0.333, 0.000}, {-0.167, -0.289}, {0.167, -0.289}};

   public:
    explicit IslandForce(double weight, const Island::Map &map) : Force(weight), map(map){};
    virtual void accum(const Fish &near) {
        (void)near;
    }
    virtual void finalize() {
        double vis = me->getVision();
        vec loc = me->getLocation();
        size_t land_cnt = 0;
        for (size_t i = 0; i < nOfSamplePoints; i++) {
            vec sample(samplePoints[i].x, samplePoints[i].y);
            vec offset = sample * vis;
            if (map.waterAt(loc + offset)) continue;
            sum -= offset / square(sample.len());
            ++land_cnt;
        }
        if (land_cnt > (nOfSamplePoints / 3) * 2 && !map.waterAt(loc)) me->kill();
    }
    virtual Force *clone() {
        Force *ptr = new IslandForce{*this};
        ptr->setMe(nullptr);
        return ptr;
    }
};

}  // namespace aq