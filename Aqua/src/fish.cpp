#include "fish.hpp"

#include <cmath>

using namespace aq;
using namespace sf;

Fish::Fish(Vector2f pos, const std::vector<Force *> &forces, float vision) : position(pos), vision(fabs(vision)) {
    this->forces.reserve(forces.size());
    for (size_t i = 0; i < forces.size(); i++) {
        Force *f = forces[i]->clone();
        f->setMe(this);
        this->forces.push_back(f);
    }
    // TODO sprite
}
Fish::Fish(const Fish &rhs) : position(rhs.position), velocity(rhs.velocity), vision(rhs.vision), sp(rhs.sp) {
    this->forces.reserve(rhs.forces.size());
    for (size_t i = 0; i < rhs.forces.size(); i++) {
        Force *f = forces[i]->clone();
        f->setMe(this);
        this->forces.push_back(f);
    }
}

void Fish::draw(RenderTarget &target) {
    sp.setPosition(position);
    sp.setRotation(atan2f(velocity.y, velocity.x) + 90);
    target.draw(sp);
}
void Fish::move(Time dt) {
    throw std::logic_error("TODO");
    (void)dt;
}
bool Fish::canSee(Fish &nb) const{
    Vector2f dist = position - nb.position;
    return vision > hypotf(dist.x, dist.y);
}
Fish::~Fish() {
    for (size_t i = 0; i < forces.size(); i++) {
        delete forces[i];
    }
}
