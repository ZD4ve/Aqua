#include "fish.hpp"

#include <gtest/gtest.h>

#include "forces.hpp"

using namespace aq;

TEST(Fish, CtorWithoutGUI) {
    EXPECT_NO_THROW(Fish basic_ctor{});
    std::vector<Force *> f;
    EXPECT_NO_THROW(Fish param_ctor(vec(0, 0), f, 10, sf::Color::Black, 0));
}