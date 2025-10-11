#include <bitset>

#include "../src/BitArray.h"
#include <gtest/gtest.h>
#include <boost/dynamic_bitset/dynamic_bitset.hpp>
using std::string;
using std::bitset;
using std::make_pair;
using boost::dynamic_bitset;

template<size_t sz>
constexpr auto create_params_with_1_bit() {
    std::array<std::pair<int, unsigned long>, sz> pairs;
    for (int i = 0; i < sz; i++) {
        pairs[i].first = sz;
        pairs[i].second = 1 << i;
    }
    return pairs;
}

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
        make_pair(15, 0),
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

TEST_P(BitArrayTest, Reset) {
    BitArray& reseted = arr->reset();
    dynamic_bitset<> expected = boost_arr->reset();
    ASSERT_PRED2(isEq, &reseted, &expected);
}

TEST_P(BitArrayTest, ResetByOne) {
    for (int i = 0; i < arr->size(); i++) {
        BitArray& reseted = arr->reset(i);
        int reseted_bit = reseted[i];
        EXPECT_EQ(reseted_bit, false);
    }
}

TEST_P(BitArrayTest, Set) {
    BitArray& seted = arr->set();
    dynamic_bitset<> expected = boost_arr->set();
    ASSERT_PRED2(isEq, &seted, &expected);
}

TEST_P(BitArrayTest, SetByOne) {
    for (int i = 0; i < arr->size(); i++) {
        BitArray& seted = arr->set(i);
        int seted_bit = seted[i];
        EXPECT_EQ(seted_bit, true);
    }
}


TEST_P(BitArrayTest, Any) {
    bool is_any = arr->any();
    bool expected = boost_arr->any();

    ASSERT_EQ(is_any, expected);
}

TEST_P(BitArrayTest, None) {
    bool is_any = arr->none();
    bool expected = boost_arr->none();

    ASSERT_EQ(is_any, expected);
}

TEST_P(BitArrayTest, Inverse) {
    BitArray inversed {arr->operator~()};
    dynamic_bitset<> expected = boost_arr->operator~();
    ASSERT_PRED2(isEq, &inversed, &expected);
}

TEST_P(BitArrayTest, Count) {
    int num_bits = arr->count();
    int expected = boost_arr->count();

    ASSERT_EQ(num_bits, expected);
}

TEST_P(BitArrayTest, Empty) {
    bool is_empty = arr->empty();
    bool expected = boost_arr->empty();

    ASSERT_EQ(is_empty, expected);
}

TEST(BitArrayTest, ResizeDown) {    //TODO test excpetions! test size < 0!!
    BitArray arr {13, 15};
    arr.resize(10);

    dynamic_bitset<> boost_arr {13, 15};
    boost_arr.resize(10);

    int expected_sz = 10;

    ASSERT_EQ(arr.size(), expected_sz);
    ASSERT_PRED2(isEq, &arr, &boost_arr);
}

TEST(BitArrayTest, ResizeUp_NoValue) {
    BitArray arr {13, 15};
    arr.resize(20);

    dynamic_bitset<> boost_arr {13, 15};
    boost_arr.resize(20);

    int expected_sz = 20;

    ASSERT_EQ(arr.size(), expected_sz);
    ASSERT_PRED2(isEq, &arr, &boost_arr);
}

TEST_P(BitArrayTest, ResizeUp_ValueTrue) {
    int sz = arr->size();
    int extra_sz = 30;
    arr->resize(sz + extra_sz, true);

    boost_arr->resize(sz + extra_sz, true);

    int expected_sz = boost_arr->size();
    int arr_sz = arr->size();

    ASSERT_EQ(arr_sz, expected_sz);
    ASSERT_PRED2(isEq, arr, boost_arr);
}

TEST_P(BitArrayTest, ResizeUp_ValueFalse) {
    int sz = arr->size();
    int extra_sz = 30;
    arr->resize(sz + extra_sz, false);

    boost_arr->resize(sz + extra_sz, false);

    int expected_sz = boost_arr->size();
    int arr_sz = arr->size();

    ASSERT_EQ(arr_sz, expected_sz);
    ASSERT_PRED2(isEq, arr, boost_arr);
}

constexpr auto one_bit_in_arr = create_params_with_1_bit<32>();
INSTANTIATE_TEST_SUITE_P(
    OneBitInArray,
    BitArrayTest,
    ::testing::ValuesIn(one_bit_in_arr)
);
