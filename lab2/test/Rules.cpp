#include <gtest/gtest.h>
#include "Core.h"
using namespace core;
TEST(Survaival, A) {
    Rules r {{2, 3, 4}, {1, 6, 8}};
    ASSERT_TRUE(r.ShouldBorn(2));
    ASSERT_TRUE(r.ShouldBorn(3));
    ASSERT_TRUE(r.ShouldBorn(4));

    ASSERT_FALSE(r.ShouldBorn(1));
    ASSERT_FALSE(r.ShouldBorn(5));
    ASSERT_FALSE(r.ShouldBorn(0));
    ASSERT_FALSE(r.ShouldBorn(-1));


    ASSERT_TRUE(r.ShouldSurvival(1));
    ASSERT_TRUE(r.ShouldSurvival(6));
    ASSERT_TRUE(r.ShouldSurvival(8));

    ASSERT_FALSE(r.ShouldSurvival(2));
    ASSERT_FALSE(r.ShouldSurvival(3));
    ASSERT_FALSE(r.ShouldSurvival(4));
    ASSERT_FALSE(r.ShouldSurvival(5));
    ASSERT_FALSE(r.ShouldSurvival(7));
    ASSERT_FALSE(r.ShouldSurvival(9));
    ASSERT_FALSE(r.ShouldSurvival(10));
}
