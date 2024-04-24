#include "fish.hpp"

#include <cmath>
#include <sstream>

using namespace aq;
using namespace sf;

sf::Texture *Fish::tex = nullptr;
size_t Fish::instance_cnt = 0;

Fish::Fish() {
    loadTexture();
    instance_cnt++;
}
Fish::Fish(sf::Vector2f pos, const std::vector<Force *> &forces, float vision, sf::Color color) : position(pos), vision(std::abs(vision)) {
    this->forces.reserve(forces.size());
    for (const auto &f : forces) {
        Force *force = f->clone();
        force->setMe(this);
        this->forces.push_back(force);
    }
    loadTexture();
    sp = sf::Sprite(tex[0]);
    sf::Vector2u tex_size = tex[0].getSize();
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
void Fish::loadTexture() {
    if (tex != nullptr) return;
    tex = new sf::Texture[n_of_animations];
    for (size_t i = 0; i < n_of_animations; i++) {
        std::ostringstream path;
        path << "asset/fish" << i << ".png";
        if (!tex[i].loadFromFile(path.str())) {
            throw std::runtime_error("Fish texture load failed!");
        }
        tex[i].generateMipmap();
        tex[i].setSmooth(false);
    }
}

void Fish::draw(RenderTarget &target) {
    constexpr float max_wait = 0.5;
    constexpr float fastest_speed = 75;
    float threshold = max_wait - (std::hypot(velocity.x, velocity.y) * max_wait / fastest_speed);
    if (last_animation_update.getElapsedTime().asSeconds() > threshold) {
        last_animation_update.restart();
        ++animation_state;
        if (animation_state == n_of_animations) animation_state = 0;
        sp.setTexture(tex[animation_state]);
    }
    sp.setPosition(position);
    sp.setRotation(std::atan2(velocity.y, velocity.x) * 180 / M_PI + 90);
    target.draw(sp);
}
bool Fish::canSee(const sf::Vector2f &pos) const {
    Vector2f dist = position - pos;
    return vision > std::hypot(dist.x, dist.y);
}
Fish::~Fish() {
    for (auto &force : forces) {
        delete force;
    }
    --instance_cnt;
    if (instance_cnt == 0) {
        delete[] tex;
        tex = nullptr;
    }
}
