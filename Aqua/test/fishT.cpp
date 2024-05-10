#include <gtest/gtest.h>

#include "fish.hpp"
#include "forces.hpp"

using namespace aq;

TEST(NoGUI, FishCtorWithoutGUI) {
    EXPECT_NO_THROW(Fish basic_ctor{});
    std::vector<Force *> f;
    EXPECT_NO_THROW(Fish param_ctor(vec(0, 0), f, 10, sf::Color::Black, 0));
}
TEST(NoGUI, FishKillWithoutGUI) {
    std::vector<Force *> f;
    Fish dummy(vec(0, 0), f, 100, sf::Color::Yellow, 0);
    EXPECT_NO_THROW(dummy.kill());
}
TEST(Fish, DeadNoMove) {
    std::vector<Force *> forces;
    forces.push_back(new CohesionForce(1));
    Fish dummy(vec(0, 0), forces, 100, sf::Color::Yellow, 0);
    Fish other(vec(0, -1), forces, 100, sf::Color::Black, 0);
    dummy.kill();
    dummy.move(sf::seconds(1), &other, &other + 1);
    EXPECT_EQ(dummy.getLocation(), vec(0, 0));
    for (auto &&f : forces) {
        delete f;
    }
}
TEST(Fish, DeadNoEffect) {
    std::vector<Force *> forces;
    forces.push_back(new CohesionForce(1));
    Fish dummy(vec(0, 0), forces, 100, sf::Color::Yellow, 0);
    std::vector<Fish> storage;
    storage.emplace_back(vec(-1, -1), forces, 100, sf::Color::Black, 0);
    storage.emplace_back(vec(+1, -1), forces, 100, sf::Color::White, 0);
    storage[0].kill();
    dummy.move(sf::seconds(1), storage.begin(), storage.end());
    EXPECT_EQ(dummy.getVelocity().norm(), vec(+1, -1).norm());
    for (auto &&f : forces) {
        delete f;
    }
}
TEST(Fish, Vision) {
    std::vector<Force *> forces;
    forces.push_back(new CohesionForce(1));
    forces.push_back(new SpeciesCohesionForce(100));
    // it wants to move more to the same species
    Fish dummy(vec(0, 0), forces, 3, sf::Color::Yellow, 0);
    std::vector<Fish> storage;
    // but can't see it
    storage.emplace_back(vec(+5, +5), forces, 100, sf::Color::Yellow, 0);
    storage.emplace_back(vec(-1, -1), forces, 100, sf::Color::Black, 1);
    dummy.move(sf::seconds(1), storage.begin(), storage.end());
    EXPECT_EQ(dummy.getVelocity().norm(), vec(-1, -1).norm());
    for (auto &&f : forces) {
        delete f;
    }
}