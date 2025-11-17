#include <gtest/gtest.h>
#include "../src/core/Core.h"
using namespace core;

TEST(Core, Rules) {
    rules r {{2, 3, 4}, {1, 6, 8}};
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

TEST(Core, Step) {
    /*      - - - - -        - - - - -
     *      - - 0 - -        - - - - -
     *      - - 0 - -   ->   - 0 0 0 -
     *      - - 0 - -        - - - - -
     *      - - - - -        - - - - -
     */
    universe u {5, 5};
    const rules rules {{3}, {2, 3}};
    u.SetRules(rules);

    u.Set({1, 2})
    .Set({2, 2})
    .Set({3, 2});

    u.Step();
    ASSERT_TRUE(u.IsAlive({2, 1}));
    ASSERT_TRUE(u.IsAlive({2, 2}));
    ASSERT_TRUE(u.IsAlive({2, 3}));

    ASSERT_FALSE(u.IsAlive({1, 2}));
    ASSERT_FALSE(u.IsAlive({3, 2}));

}
