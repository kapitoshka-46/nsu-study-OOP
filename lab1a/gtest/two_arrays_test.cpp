#include <bitset>

#include "../src/BitArray.h"
#include <gtest/gtest.h>
#include <boost/dynamic_bitset/dynamic_bitset.hpp>
using std::string;
using std::bitset;
using std::make_pair;
using std::make_tuple;
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

class TwoBitArrayTest : public ::testing::TestWithParam<std::tuple<int, unsigned long, int, unsigned long>> {
public:
    BitArray *arr1 {};
    BitArray *arr2 {};
    dynamic_bitset<> * boost_arr1 {};
    dynamic_bitset<> * boost_arr2 {};

    void SetUp() override {
        auto [sz1, value1, sz2, value2] = GetParam();
        arr1 = new BitArray{sz1, value1};
        arr2 = new BitArray{sz2, value2};
        boost_arr1 = new dynamic_bitset(sz1, value1);
        boost_arr2 = new dynamic_bitset(sz2, value2);
    }
    void TearDown() override {
        delete arr1;
        delete arr2;
        delete boost_arr1;
        delete boost_arr2;
    }
};

class TwoBitArrayTestSameSize : public ::testing::TestWithParam<std::tuple<int, unsigned long, unsigned long>> {
public:
    BitArray *arr1 {};
    BitArray *arr2 {};
    dynamic_bitset<> * boost_arr1 {};
    dynamic_bitset<> * boost_arr2 {};

    void SetUp() override {
        auto [sz, value1, value2] = GetParam();
        arr1 = new BitArray{sz, value1};
        arr2 = new BitArray{sz, value2};
        boost_arr1 = new dynamic_bitset(sz, value1);
        boost_arr2 = new dynamic_bitset(sz, value2);
    }
    void TearDown() override {
        delete arr1;
        delete arr2;
        delete boost_arr1;
        delete boost_arr2;
    }
};

bool isEq(const BitArray *a, const dynamic_bitset<> *b);

TEST_P(TwoBitArrayTest, Compare) {
    bool expected = boost_arr1 == boost_arr2;
    bool result_of_compare = arr1 == arr2;
    ASSERT_EQ(result_of_compare, expected);
    ASSERT_EQ(arr1 != arr2, not expected);
}


TEST_P(TwoBitArrayTestSameSize, Compare) {
    bool expected = boost_arr1 == boost_arr2;
    bool result_of_compare = arr1 == arr2;
    ASSERT_EQ(result_of_compare, expected);
    ASSERT_EQ(arr1 != arr2, not expected);
}

TEST_P(TwoBitArrayTestSameSize, And) {
    BitArray result = *arr1 & *arr2;
    dynamic_bitset<> expected = *boost_arr1 & *boost_arr2;

    ASSERT_PRED2(isEq, &result, &expected);
}
TEST_P(TwoBitArrayTestSameSize, Or) {
    BitArray result = *arr1 | *arr2;
    dynamic_bitset<> expected = *boost_arr1 | *boost_arr2;

    std::string expected_str;
    boost::to_string(expected, expected_str);

    ASSERT_PRED2(isEq, &result, &expected);
}
TEST_P(TwoBitArrayTestSameSize, XOR) {
    BitArray result = *arr1 ^ *arr2;
    dynamic_bitset<> expected = *boost_arr1 ^ *boost_arr2;

    ASSERT_PRED2(isEq, &result, &expected);
}


TEST_P(TwoBitArrayTestSameSize, AndAssign) {
    BitArray tmp_arr1 {*arr1};
    BitArray tmp_arr2 {*arr2};
    BitArray expected = tmp_arr1 & tmp_arr2;
    *arr1 &= *arr2;

    ASSERT_EQ(arr1->size(), arr2->size());
    ASSERT_TRUE(*arr1 == expected);
    ASSERT_TRUE(*arr2 == tmp_arr2);
}

TEST_P(TwoBitArrayTestSameSize, OrAssign) {
    BitArray tmp_arr1 {*arr1};
    BitArray tmp_arr2 {*arr2};
    BitArray expected = tmp_arr1 | tmp_arr2;
    *arr1 |= *arr2;

    ASSERT_EQ(arr1->size(), arr2->size());
    ASSERT_TRUE(*arr1 == expected);
    ASSERT_TRUE(*arr2 == tmp_arr2);
}

TEST_P(TwoBitArrayTestSameSize, XorAssign) {
    BitArray tmp_arr1 {*arr1};
    BitArray tmp_arr2 {*arr2};
    BitArray expected = tmp_arr1 ^ tmp_arr2;
    *arr1 ^= *arr2;

    ASSERT_EQ(arr1->size(), arr2->size());
    ASSERT_TRUE(*arr1 == expected);
    ASSERT_TRUE(*arr2 == tmp_arr2);
}


TEST_P(TwoBitArrayTest, AndDiffSize) {
    if (arr1->size() != arr2->size()) {
        ASSERT_ANY_THROW(*arr1 & *arr2);
    }
}
TEST_P(TwoBitArrayTest, OrDiffSize) {
    if (arr1->size() != arr2->size()) {
        ASSERT_ANY_THROW(*arr1 | *arr2);
    }
}
TEST_P(TwoBitArrayTest, XorDiffSize) {
    if (arr1->size() != arr2->size()) {
        ASSERT_ANY_THROW(*arr1 ^ *arr2);
    }
}

TEST_P(TwoBitArrayTest, Swap) {
    BitArray prev_arr1 {*arr1};
    BitArray prev_arr2 {*arr2};

    arr1->swap(*arr2);

    // check arr1
    ASSERT_EQ(arr1->size(), prev_arr2.size());
    ASSERT_PRED2(BitArray::is_equal, *arr1, prev_arr2);

    // check arr2
    ASSERT_EQ(arr2->size(), prev_arr1.size());
    ASSERT_PRED2(BitArray::is_equal, *arr2, prev_arr1);
}

TEST_P(TwoBitArrayTest, DoubleSwap) {
    BitArray prev_arr1 {*arr1};
    BitArray prev_arr2 {*arr2};

    arr1->swap(*arr2);
    arr1->swap(*arr2);

    // check arr1
    ASSERT_EQ(arr1->size(), prev_arr1.size());
    ASSERT_PRED2(BitArray::is_equal, *arr1, prev_arr1);

    // check arr2
    ASSERT_EQ(arr2->size(), prev_arr2.size());
    ASSERT_PRED2(BitArray::is_equal, *arr2, prev_arr2);
}

INSTANTIATE_TEST_SUITE_P(
    MinMaxValeus,
    TwoBitArrayTest,
    ::testing::Values(
        make_tuple(0,  0,       0,  0),
        make_tuple(8,  0xFF,    0,  0),
        make_tuple(16, 0xFFFF,  0,  0),
        make_tuple(16, 0xFFFF,  16, 0xFFFF)
    )
);

INSTANTIATE_TEST_SUITE_P(
    Values,
    TwoBitArrayTest,
    ::testing::Values(
        make_tuple(125, 321301123, 25120,  2410515),
        make_tuple(5215, 60066060 ,660606, 222222222),
        make_tuple(99999999, 0xFFFFFFFF,  1111111111,  1111111111),
        make_tuple(16, 0xFFFF,  16, 0xFFFF),
        make_tuple(0, 0, 12512, 3213),

        make_tuple(125, 3333, 125, 333),
        make_tuple(5215, 11060, 5215, 6066060),
        make_tuple(255, 99999999, 255, 0xFFFFFFFF),
        make_tuple(16, 0xFFFF, 16, 0xFF),
        make_tuple(10, 30, 10, 30),
        make_tuple(16, 0xFFFF, 16, 0),
        make_tuple(0, 0, 0, 0)
    )
);

INSTANTIATE_TEST_SUITE_P(
    SameValuesAndSize,
    TwoBitArrayTestSameSize,
    ::testing::Values(
        make_tuple(125, 3333,  333),
        make_tuple(5215, 11060, 6066060),
        make_tuple(255, 99999999, 0xFFFFFFFF),
        make_tuple(16, 0xFFFF, 0xFF),
        make_tuple(10, 30, 30),
        make_tuple(16, 0xFFFF, 0),
        make_tuple(0, 0, 0)
    )
);