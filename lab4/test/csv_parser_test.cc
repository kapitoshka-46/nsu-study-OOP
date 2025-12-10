#include <gtest/gtest.h>
#include <sstream>
#include "../csv_parser.h"

using namespace std;
using TripleString = tuple<string,string, string>;

TEST(CSVParser, BasicParsing) {
    string records = "aaa,bbb,ccc\n"
                     "zzz,yyy,xxx";
    istringstream iss(records);
    CSVParser<string, string, string> csv {iss};

    TripleString expected1 = {"aaa", "bbb", "ccc"};
    TripleString expected2 = {"zzz", "yyy", "xxx"};

    csv.Next();
    ASSERT_EQ(csv.Value(), expected1);

    csv.Next();
    ASSERT_EQ(csv.Value(), expected2);
}


TEST(CSVParser, CallNextMoreThenPossible) {
    string records = "aaa,bbb,ccc\n"
                     "zzz,yyy,xxx";
    istringstream iss(records);
    CSVParser<string, string, string> csv {iss};

    TripleString expected1 = {"aaa", "bbb", "ccc"};
    TripleString expected2 = {"zzz", "yyy", "xxx"};

    ASSERT_TRUE(csv.Next());
    ASSERT_TRUE(csv.Next());

    ASSERT_FALSE(csv.Next());
}

TEST(CSVParser, ValueWithoutNext) {
    string records = "aaa,bbb,ccc\n";
    istringstream iss(records);
    CSVParser<string, string, string> csv {iss};

    TripleString expected = {};
    ASSERT_EQ(csv.Value(), expected);   //TODO: а что в этом случае лучше делать?
}
