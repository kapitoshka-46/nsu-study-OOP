#include <gtest/gtest.h>
#include "../src/core/Core.h"

using namespace core;
TEST(Constructor, DistanceSize) {
    for (int i = 1; i < 100; i++) {
        for (int j = 1; j < 100; j++) {
            universe u {i,  j};
            ASSERT_EQ(u.Rows(), i);
            ASSERT_EQ(u.Cols(), j);
        }
    }
}

TEST(Constructor, ZeroOrNegativeSize) {
    ASSERT_THROW(universe(0,0), std::invalid_argument);
    ASSERT_THROW(universe(-20, 20), std::invalid_argument);
    ASSERT_THROW(universe(10, -5), std::invalid_argument);
    ASSERT_THROW(universe(-1, -1), std::invalid_argument);

}

TEST(GetTickCount, AfterSteps) {
    universe u {10,30};
    int expected = 0;
    for (int i = 0; i <  100; i++) {
        ASSERT_EQ(u.GetTickCount(), expected++);
        u.Step();
    }
    ASSERT_EQ(u.GetTickCount(), expected++);
}

TEST(Name, GetSet) {
    universe u{20, 30};
    std::string expected = "abacaba";
    u.SetName(expected);
    ASSERT_EQ(u.GetName(), expected);
}
