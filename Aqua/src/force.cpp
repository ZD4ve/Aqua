#include <fish.hpp>
#include <force.hpp>

using namespace aq;

void Force::setMe(Fish *me) {
    if (this->me != nullptr && me != nullptr) throw std::logic_error("Already assigned fish to force!");
    this->me = me;
}

vec Force::getSum() {
    vec tmp = sum;
    sum = vec(0, 0);
    return tmp * weight;
}