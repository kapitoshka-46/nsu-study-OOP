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

TEST(CSVParser, OneRecordCRLF) {
    string record ="aaa,bbb,ccc\n";
    istringstream iss(record);
    CSVParser<string, string, string> csv {iss};
    TripleString expected = {"aaa", "bbb", "ccc"};
    csv.Next();
    ASSERT_EQ(csv.Value(), expected);
}

TEST(CSVParser, OneRecordNoCRLF) {
    string record ="aaa,bbb,ccc";   // <-- no '\n'
    istringstream iss(record);
    CSVParser<string, string, string> csv {iss};
    TripleString expected = {"aaa", "bbb", "ccc"};
    csv.Next();
    ASSERT_EQ(csv.Value(), expected);
}

TEST(CSVParser, QuotedCRLF) {
    string record = "aaa,"  "before_\"text\ntext\"_after,"   "ccc\n";
    istringstream iss(record);
    CSVParser<string, string, string> csv {iss};
    TripleString expected = {"aaa", "before_text\ntext_after", "ccc"};
}

TEST(CSVParser, UnclosedQuoteNoCRLF) {
    string record = "aaa, before_\"quotestart, cccc";
    istringstream iss(record);
    CSVParser<string, string, string> csv {iss};
    TripleString expected = {"aaa", "before_quote", "ccc"};
    ASSERT_THROW(csv.Next(), CSVParserException);

}

TEST(CSVParser, UnclosedQuoteCRLF) {
    string record = "aaa, before_\"quotestart\nnewline, cccc";
    istringstream iss(record);
    CSVParser<string, string, string> csv {iss};
    ASSERT_THROW(csv.Next(), ParseException);

}