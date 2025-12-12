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
    TripleString t;

    csv >> t;
    ASSERT_EQ(t, expected1);
    csv >> t;
    ASSERT_EQ(t, expected2);
}


TEST(CSVParser, CallNextMoreThenPossible) {
    string records = "aaa,bbb,ccc\n"
                     "zzz,yyy,xxx";
    istringstream iss(records);
    CSVParser<string, string, string> csv {iss};
    TripleString t;

    ASSERT_TRUE(csv.operator>>(t));
    ASSERT_TRUE(csv.operator>>(t));
    ASSERT_FALSE(csv.operator>>(t));
}

TEST(CSVParser, QuoteToTheEnd) {
    string record = "a,b,\"ccc, cc, ccc\"";
    istringstream iss(record);
    CSVParser<string, string, string> csv {iss};

    TripleString expected = {"a","b","ccc, cc, ccc"};
    TripleString t;
    csv >> t;
    ASSERT_EQ(t, expected);
}

TEST(CSVParser, QuoteNewLineEnd) {
    string record = "a,b,\"ccc,\n\n cc,\n c\ncc\"";
    istringstream iss(record);
    CSVParser<string, string, string> csv {iss};

    TripleString expected = {"a","b","ccc,\n\n cc,\n c\ncc"};
    TripleString t;
    csv >> t;
    ASSERT_EQ(t, expected);
}

TEST(CSVParser, OneRecordCRLF) {
    string record ="aaa,bbb,ccc\n";
    istringstream iss(record);
    CSVParser<string, string, string> csv {iss};
    TripleString expected = {"aaa", "bbb", "ccc"};

    TripleString actual;
    csv >> actual;
    ASSERT_EQ(actual, expected);
}

TEST(CSVParser, OneRecordNoCRLF) {
    string record ="aaa,bbb,ccc";   // <-- no '\n'
    istringstream iss(record);
    CSVParser<string, string, string> csv {iss};
    TripleString expected = {"aaa", "bbb", "ccc"};

    TripleString actual;
    csv >> actual;
    ASSERT_EQ(actual, expected);
}

TEST(CSVParser, CommaAfterQuote) {
    string record ="aaa,\"bbb\",ccc";   // <-- no '\n'
    istringstream iss(record);
    CSVParser<string, string, string> csv {iss};
    TripleString expected = {"aaa", "bbb", "ccc"};
    TripleString actual;
    csv >> actual;
    ASSERT_EQ(actual, expected);
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
    TripleString t;
    ASSERT_THROW(csv.operator>>(t), CSVParserException);

}

TEST(CSVParser, UnclosedQuoteCRLF) {
    string record = "aaa, before_\"quotestart\nnewline, cccc";
    istringstream iss(record);
    CSVParser<string, string, string> csv {iss};
    TripleString t;
    ASSERT_THROW(csv.operator>>(t), ParseException);

}