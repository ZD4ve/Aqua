#include <gtest/gtest.h>

#include "fish.hpp"

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}
/*
Tesztelesi Terv:

1.
A GUI-tól független erők tesztelése:
Minden erőhöz venni 2-3 halat, és megnézni,
hogy milyen irányba mozdulnak el, különböző paraméterekkel.

2.
Halott hal nem mozog, és nem hat másokra.
*/