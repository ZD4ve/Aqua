#include <gtest/gtest.h>

#include "fish.hpp"
#include "forces.hpp"

using namespace aq;

TEST(Force, SeparationForce) {
    std::vector<Force *> forces;
    forces.push_back(new SeparationForce(1, 10));
    Fish dummy(vec(0, 0), forces, 100, sf::Color::Yellow, 0);
    std::vector<Fish> storage;
    storage.emplace_back(vec(-1, -1), forces, 100, sf::Color::Black, 0);
    storage.emplace_back(vec(+1, -1), forces, 100, sf::Color::Black, 0);
    dummy.move(sf::seconds(1), storage.begin(), storage.end());
    EXPECT_EQ(dummy.getVelocity().norm(), vec(0, 1));
    for (auto &&f : forces) {
        delete f;
    }
}
TEST(Force, AlignmentForce) {
    std::vector<Force *> empty;
    std::vector<Force *> fear;
    std::vector<Force *> align;
    fear.push_back(new SeparationForce(1, 100));
    align.push_back(new AlignmentForce(1));

    Fish dummy(vec(0, 0), align, 1000, sf::Color::Yellow, 0);
    Fish evil(vec(-1, -1), empty, 100, sf::Color::Red, 0);
    std::vector<Fish> storage;
    storage.emplace_back(vec(-1, 0), fear, 100, sf::Color::Black, 0);
    storage.emplace_back(vec(0, -1), fear, 100, sf::Color::Black, 0);
    for (auto &&fish : storage) {
        // get the neighbors moving
        fish.move(sf::milliseconds(100), &evil, &evil + 1);
    }
    dummy.move(sf::seconds(1), storage.begin(), storage.end());

    EXPECT_EQ(dummy.getVelocity().norm(), vec(1, 1).norm());

    for (auto &&f : fear) {
        delete f;
    }
    for (auto &&f : align) {
        delete f;
    }
}
TEST(Force, CohesionForce) {
    std::vector<Force *> forces;
    forces.push_back(new CohesionForce(1));
    Fish dummy(vec(0, 0), forces, 100, sf::Color::Yellow, 0);
    std::vector<Fish> storage;
    storage.emplace_back(vec(-1, -1), forces, 100, sf::Color::Black, 0);
    storage.emplace_back(vec(+0, -1), forces, 100, sf::Color::Black, 0);
    storage.emplace_back(vec(+1, -1), forces, 100, sf::Color::Black, 0);
    dummy.move(sf::seconds(1), storage.begin(), storage.end());
    EXPECT_EQ(dummy.getVelocity().norm(), vec(0, -1));
    for (auto &&f : forces) {
        delete f;
    }
}
TEST(Force, SpeciesCohesionForce) {
    std::vector<Force *> forces;
    forces.push_back(new SpeciesCohesionForce(1));
    Fish dummy(vec(0, 0), forces, 100, sf::Color::Yellow, 0);
    std::vector<Fish> storage;
    storage.emplace_back(vec(-1, -1), forces, 100, sf::Color::Yellow, 0);
    storage.emplace_back(vec(+1, -1), forces, 100, sf::Color::Yellow, 0);
    storage.emplace_back(vec(+5, -1), forces, 100, sf::Color::Black, 1);
    storage.emplace_back(vec(-2, -1), forces, 100, sf::Color::White, 2);

    dummy.move(sf::seconds(1), storage.begin(), storage.end());
    EXPECT_EQ(dummy.getVelocity().norm(), vec(0, -1));
    for (auto &&f : forces) {
        delete f;
    }
}