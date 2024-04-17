#include "fish.hpp"

#include <cmath>

using namespace aq;
using namespace sf;

sf::Texture *Fish::tex = nullptr;
size_t Fish::instance_cnt = 0;

Fish::Fish() {
    instance_cnt++;
}

Fish::Fish(sf::Vector2f pos, const std::vector<Force *> &forces, float vision, sf::Color color) : position(pos), vision(std::abs(vision)) {
    this->forces.reserve(forces.size());
    for (const auto &f : forces) {
        Force *force = f->clone();
        force->setMe(this);
        this->forces.push_back(force);
    }
    if (tex == nullptr) {
        tex = new sf::Texture();
        if (!tex->loadFromFile("asset/fish.png")) {
            throw std::runtime_error("Fish texture load fail!");
        }
        tex->setSmooth(false);
    }
    sp = sf::Sprite(*tex);
    sf::Vector2u tex_size = tex->getSize();
    sp.setOrigin(tex_size.x / 2.0F, tex_size.y / 2.0F);
    sp.setScale(sf::Vector2f(5.0 / tex_size.y, 5.0 / tex_size.y));
    sp.setColor(color);
    ++instance_cnt;
}
Fish::Fish(const Fish &rhs) : position(rhs.position), velocity(rhs.velocity), vision(rhs.vision), sp(rhs.sp) {
    this->forces.reserve(rhs.forces.size());
    for (const auto &f : rhs.forces) {
        Force *force = f->clone();
        force->setMe(this);
        this->forces.push_back(force);
    }
    ++instance_cnt;
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
    sp.setRotation(atan2(velocity.y, velocity.x) * 180 / M_PI + 90);
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
    --instance_cnt;
    if (instance_cnt == 0) {
        delete tex;
        tex = nullptr;
    }
}
