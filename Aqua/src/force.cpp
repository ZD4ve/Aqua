#include <fish.hpp>
#include <force.hpp>

using namespace aq;
using sf::Vector2f;

void Force::setMe(Fish *me) {
    if (me != nullptr) throw std::logic_error("Already assigned fish to force!");
    this->me = me;
}

Vector2f Force::getSum() {
    Vector2f tmp = sum;
    sum = Vector2f(0, 0);
    return tmp * weight;
}