#include <gtest/gtest.h>
#include "../src/Core.h"

using namespace core;
TEST(Constructor, DistanceSize) {
    for (int i = 1; i < 100; i++) {
        for (int j = 1; j < 100; j++) {
            Universe u {i,  j};
            ASSERT_EQ(u.Rows(), i);
            ASSERT_EQ(u.Cols(), j);
        }
    }
}

TEST(Constructor, ZeroOrNegativeSize) {
    ASSERT_THROW(Universe(0,0), std::invalid_argument);
    ASSERT_THROW(Universe(-20, 20), std::invalid_argument);
    ASSERT_THROW(Universe(10, -5), std::invalid_argument);
    ASSERT_THROW(Universe(-1, -1), std::invalid_argument);

}

TEST(GetTickCount, AfterSteps) {
    Universe u {10,30};
    int expected = 0;
    for (int i = 0; i <  100; i++) {
        ASSERT_EQ(u.GetTickCount(), expected++);
        u.Step();
    }
    ASSERT_EQ(u.GetTickCount(), expected++);
}

TEST(Name, GetSet) {
    Universe u{20, 30};
    std::string expected = "abacaba";
    u.SetName(expected);
    ASSERT_EQ(u.GetName(), expected);
}
