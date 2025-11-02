#include <gtest/gtest.h>
#include "../src/Core.h"
#include <tuple>

using enum CellType;
using std::string;
using std::make_tuple;

string boat_str =   "-----"     // 5 x 5
                    "-##--"
                    "-#-#-"
                    "--#--"
                    "-----";
string square_str =     "####"  // 4 x 4
                        "#--#"
                        "#--#"
                        "####";

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

class LittleFields : public ::testing::Test {
public:
    Field boat {boat_str, 5, 5};   // creating boat
    Field square {square_str, 4, 4};
};


TEST(CountAliveAt, f1) {
    Field f  {"---#------"
                  "---####---"
                  "-#--------", 3, 10};
    ASSERT_EQ(f.CountAliveAt({1, 3}), 2);
}

TEST(CountAliveAt, f2) {
    Field f  {"---#------"
                  "---####---"
                  "-#--------", 3, 10};
    ASSERT_EQ(f.CountAliveAt({0, 9}), 0);
}

TEST_F(LittleFields, CountAlive) {
    CellPos pos {2, 2};
    int alive = boat.CountAliveAt(pos);
    int expected = 5;

    ASSERT_EQ(alive, expected) << "at " << pos;

    pos = {3,3};
    alive = boat.CountAliveAt(pos);
    expected = 2;

    ASSERT_EQ(alive, expected) << "at " << pos;

    pos = {4,2};
    alive = boat.CountAliveAt(pos);
    expected = 1;

    ASSERT_EQ(alive, expected) << "at " << pos;

    pos = {1,4};
    alive = boat.CountAliveAt(pos);
    expected = 1;

    ASSERT_EQ(alive, expected) << "at " << pos;


    pos = {0,0};
    alive = square.CountAliveAt(pos);
    expected = 7;
    ASSERT_EQ(alive, expected) << "at " << pos;

    pos = {1,0};
    alive = square.CountAliveAt(pos);
    expected = 6;
    ASSERT_EQ(alive, expected) << "at " << pos;

}

TEST(Getters, Sizes) {
    Field f {"1111""2222""3333", 3, 4};
    ASSERT_EQ(f.Rows(), 3);
    ASSERT_EQ(f.Cols(), 4);

}
TEST(Getters, CellState_OutOfRange) {
    Field field {boat_str, 5, 5};

    ASSERT_THROW(field.GetCellState({-1, 2}), std::out_of_range);
    ASSERT_THROW(field.GetCellState({5, 5}), std::out_of_range);
    ASSERT_THROW(field.GetCellState({4, 5}), std::out_of_range);
    ASSERT_THROW(field.GetCellState({5, 2}), std::out_of_range);


}

TEST_F(LittleFields, Get) {
    CellType cell;
    CellPos pos{};
    CellType expected;
    for (int col = 0; col < boat.Rows(); col++) {
        pos = {0, col};
        cell = boat.GetCellState({0, col});
        expected = kDead;

        ASSERT_EQ(cell, expected) << "pos =" << pos;

        pos = {4,0};
        cell = boat.GetCellState(pos);

        ASSERT_EQ(cell, expected) << "pos =" << pos;
    }
    ASSERT_EQ(boat.GetCellState({1,0}), kDead);
    ASSERT_EQ(boat.GetCellState({1,1}), kAlive);
    ASSERT_EQ(boat.GetCellState({1,2}), kAlive);
    ASSERT_EQ(boat.GetCellState({1,3}), kDead);
    ASSERT_EQ(boat.GetCellState({1,4}), kDead);

    ASSERT_EQ(boat.GetCellState({2,0}), kDead);
    ASSERT_EQ(boat.GetCellState({2,1}), kAlive);
    ASSERT_EQ(boat.GetCellState({2,2}), kDead);
    ASSERT_EQ(boat.GetCellState({2,3}), kAlive);
    ASSERT_EQ(boat.GetCellState({2,4}), kDead);


    ASSERT_EQ(boat.GetCellState({3,0}), kDead);
    ASSERT_EQ(boat.GetCellState({3,1}), kDead);
    ASSERT_EQ(boat.GetCellState({3,2}), kAlive);
    ASSERT_EQ(boat.GetCellState({3,3}), kDead);
    ASSERT_EQ(boat.GetCellState({3,4}), kDead);

}

TEST_F(LittleFields, Set) {
    for (int row = 0; row < boat.Rows(); row++) {
        for (int col = 0; col < boat.Cols(); col++) {
            boat.Set({row, col}, kAlive);
            ASSERT_EQ(boat.GetCellState({row, col}), kAlive);
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
