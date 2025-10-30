#include <gtest/gtest.h>
#include "../src/Core.h"
#include <tuple>

using enum CellType;
using std::string;
using std::make_tuple;

string boat_str = "-----"
                "-##--"
                "-#-#-"
                "--#--"
                "-----";

class FieldTest : public ::testing::TestWithParam<std::tuple<std::string, int, int>> {
public:
    Field *field;
    int rows;
    int cols;

    void SetUp() override {
        auto [s, r, c] = GetParam();
        field = new Field(s, r, c);
        rows = r;
        cols = c;
    }
    void TearDown() override {
    }
};

class Boat : public ::testing::Test {
public:
    Field field {boat_str, 5, 5};
    int rows {5};
    int cols {5};

    void SetUp() override {

    }
    void TearDown() override {
    }
};

TEST_F(Boat, CountAlive) {

}

TEST_F(Boat, Get) {
    CellType cell;
    CellPos pos{};
    CellType expected;
    for (int col = 0; col < rows; col++) {
        pos = {0, col};
        cell = field.GetCellState({0, col});
        expected = kDead;

        ASSERT_EQ(cell, expected) << "pos =" << pos;

        pos = {4,0};
        cell = field.GetCellState(pos);

        ASSERT_EQ(cell, expected) << "pos =" << pos;
    }
    ASSERT_EQ(field.GetCellState({1,0}), kDead);
    ASSERT_EQ(field.GetCellState({1,1}), kAlive);
    ASSERT_EQ(field.GetCellState({1,2}), kAlive);
    ASSERT_EQ(field.GetCellState({1,3}), kDead);
    ASSERT_EQ(field.GetCellState({1,4}), kDead);

    ASSERT_EQ(field.GetCellState({2,0}), kDead);
    ASSERT_EQ(field.GetCellState({2,1}), kAlive);
    ASSERT_EQ(field.GetCellState({2,2}), kDead);
    ASSERT_EQ(field.GetCellState({2,3}), kAlive);
    ASSERT_EQ(field.GetCellState({2,4}), kDead);


    ASSERT_EQ(field.GetCellState({3,0}), kDead);
    ASSERT_EQ(field.GetCellState({3,1}), kDead);
    ASSERT_EQ(field.GetCellState({3,2}), kAlive);
    ASSERT_EQ(field.GetCellState({3,3}), kDead);
    ASSERT_EQ(field.GetCellState({3,4}), kDead);

}

TEST_F(Boat, Set) {
    for (int row = 0; row < field.SizeRows(); row++) {
        for (int col = 0; col < field.SizeCols(); col++) {
            field.Set({row, col}, kAlive);
            ASSERT_EQ(field.GetCellState({row, col}), kAlive);
        }
    }
}
//
// INSTANTIATE_TEST_SUITE_P(
//     TestValues,
//     FieldTest,
//     ::testing::Values(
//     make_tuple(
//                 "-----"
//                 "-##--"
//                 "-#-#-"
//                 "--#--"
//                 "-----", 5, 5)
//     )
// );
