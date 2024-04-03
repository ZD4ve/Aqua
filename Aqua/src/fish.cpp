#include "fish.hpp"

#include <cmath>

using namespace aq;
using namespace sf;

Fish::Fish(Vector2f pos, const std::vector<Force *> &forces, float vision) : position(pos), vision(std::abs(vision)) {
    this->forces.reserve(forces.size());
    for (const auto &f : forces) {
        Force *force = f->clone();
        force->setMe(this);
        this->forces.push_back(force);
    }
    // TODO: sprite
}
Fish::Fish(const Fish &rhs) : position(rhs.position), velocity(rhs.velocity), vision(rhs.vision), sp(rhs.sp) {
    this->forces.reserve(rhs.forces.size());
    for (const auto &f : rhs.forces) {
        Force *force = f->clone();
        force->setMe(this);
        this->forces.push_back(force);
    }
}
Fish &Fish::operator=(const Fish &rhs) {
    if (this == &rhs) return *this;
    velocity = rhs.velocity;
    position = rhs.position;
    vision = rhs.vision;
    sp = rhs.sp;
    for (size_t i = 0; i < forces.size(); i++) {
        delete this->forces[i];
    }
    this->forces.clear();
    this->forces.reserve(rhs.forces.size());
    for (const auto &f : rhs.forces) {
        Force *force = f->clone();
        force->setMe(this);
        this->forces.push_back(force);
    }
    return *this;
}

void Fish::draw(RenderTarget &target) {
    sp.setPosition(position);
    sp.setRotation(atan2f(velocity.y, velocity.x) + 90);
    target.draw(sp);
}
bool Fish::canSee(Fish &near) const {
    Vector2f dist = position - near.position;
    return vision > hypotf(dist.x, dist.y);
}
Fish::~Fish() {
    for (auto &force : forces) {
        delete force;
    }
}
