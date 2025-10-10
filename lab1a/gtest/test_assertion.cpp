#include <bitset>

#include "../src/BitArray.h"
#include <gtest/gtest.h>
#include <boost/dynamic_bitset/dynamic_bitset.hpp>
using std::string;
using std::bitset;
using std::make_pair;
using boost::dynamic_bitset;

class BitArrayTest : public ::testing::TestWithParam<std::pair<int, unsigned long>> {
public:
    BitArray *arr {};
    dynamic_bitset<> * boost_arr {};
    void SetUp() override {
        auto [sz, value] = GetParam();
        arr = new BitArray{sz, value};
        boost_arr = new dynamic_bitset(sz, value);
    }
    void TearDown() override {
        delete arr;
        delete boost_arr;
    }
};

bool isEq(const BitArray *a, const dynamic_bitset<> *b) {
    string b_str;
    boost::to_string(*b, b_str);
    return a->to_string() == b_str;
}

TEST_P(BitArrayTest, InitialSize) {

    int arr_size = arr->size();
    int expected = boost_arr->size();

    ASSERT_EQ(arr_size, expected);

}

TEST_P(BitArrayTest, ToString) {
    string arr_str = arr->to_string();
    string expected;
    to_string(*boost_arr, expected);

    ASSERT_EQ(arr_str, expected);
}

TEST_P(BitArrayTest, Clear) {
    arr->clear();
    boost_arr->clear();

    int expected_size = 0;
    int arr_size = arr->size();
    ASSERT_EQ(arr_size, expected_size);
    ASSERT_PRED2(isEq, arr, boost_arr);
}

TEST_P(BitArrayTest, PushBackTrueBit) {
    arr->push_back(true);
    bool expected = true;

    bool last_bit = arr->operator[](arr->size()-1);

    ASSERT_EQ(last_bit, expected);
}

INSTANTIATE_TEST_SUITE_P(
    ForInitialSize,
    BitArrayTest,
    ::testing::Values(
        make_pair(5, 13),
        make_pair(15, 123),
        make_pair(27, 9120),
        make_pair(99, 999999999),
        make_pair(50, 4294967295)
    )
);

INSTANTIATE_TEST_SUITE_P(
    MinMaxValues,
    BitArrayTest,
    ::testing::Values(
        make_pair(0, 0),
        make_pair(8, 0xFF),
        make_pair(8, 0),
        make_pair(16, 0xFFFF),
        make_pair(16, 0)
    )
);

TEST(BitArray, Swap) {
}