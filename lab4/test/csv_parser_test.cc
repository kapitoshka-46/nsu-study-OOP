#include <gtest/gtest.h>
#include <sstream>
#include "../csv_parser.h"

using namespace std;
using TripleString = tuple<string,string, string>;

using TripleWString = tuple<wstring,wstring, wstring>;

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

TEST(CSVParser, EscapingQuote) {
    string record = "a,b,cc\\\"cc";
    istringstream iss(record);
    CSVParser<string, string, string> csv {iss};
    TripleString t;
    csv >> t;
    TripleString expected = {"a", "b", "cc\"cc"};
    ASSERT_EQ(t, expected);
}

TEST(CSVParser, EscapingComma) {
    string record = "a,b,cc\\,cc";
    istringstream iss(record);
    CSVParser<string, string, string> csv {iss};
    TripleString t;
    csv >> t;
    TripleString expected = {"a", "b", "cc,cc"};
    ASSERT_EQ(t, expected);
}

TEST(CSVParser, EscapingNewLine) {
    string record = "a,b,cc\\\ncc";
    istringstream iss(record);
    CSVParser<string, string, string> csv {iss};
    TripleString t;
    csv >> t;
    TripleString expected = {"a", "b", "cc\ncc"};
    ASSERT_EQ(t, expected);
}

TEST(CSVParser, EscapingChar) {
    string record = "a,b,cc\\xcc";
    istringstream iss(record);
    CSVParser<string, string, string> csv {iss};
    TripleString t;
    csv >> t;
    TripleString expected = {"a", "b", "ccxcc"};
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

TEST(CSVParser, Empty) {
    string record = "";
    istringstream iss(record);
    CSVParser<string, string, string> csv {iss};
    TripleString actual = {"la", "la", "la"};

    TripleString expected = actual;

    csv >> actual;
    ASSERT_EQ(actual,expected);
}

TEST(CSVParser, IteratorStyleFor) {
    string record = "1\n2\n3\n4\n5\n6\n7\n8\n9\n10";
    istringstream iss(record);
    CSVParser<int> csv {iss};
    std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> actual;
    for (tuple<int> t : csv) {
        actual.push_back(std::get<0>(t));
    }
    ASSERT_EQ(actual, expected);
}

TEST(CSVParser, DifferentConfig) {
    string record ="aaa|bbb|ccc\tddd|eee|ff\'ff\t\'";   // <-- no '\n'
    istringstream iss(record);
    CSVParser<string, string, string> csv {iss, 0, '|', '\t', '\''};
    TripleString actual;

    csv >> actual;
    TripleString expected = {"aaa", "bbb", "ccc"};
    ASSERT_EQ(actual, expected);

    csv >> actual;
    expected = {"ddd", "eee", "ffff\t"};
    ASSERT_EQ(actual, expected);
}

TEST(CSVParser, NewLine) {
    string record = "1, 11\n";
    istringstream iss(record);
    CSVParser<int, int> csv {iss};
    std::tuple<int, int> actual;
    for (auto t : csv) {
        actual = t;
    }
    std::tuple<int, int> expected = std::make_tuple(1, 11);
    ASSERT_EQ(actual, expected);

}

TEST(CSVParser, SkipLines) {
    string record ="aaa,bbb,ccc\n"
                   "ddd,eee,fff\n"
                   "ggg,hhh,iii\n";   // <-- no '\n'
    istringstream iss(record);
    CSVParser<string, string, string> csv {iss, 2};
    TripleString expected = {"ggg", "hhh", "iii"};

    TripleString actual;
    csv >> actual;
    ASSERT_EQ(actual, expected);
}

TEST(BasicCSVParser, WCharBasicParsing) {
    wstring records = L"aaa,bbb,ccc\n"
                 "zzz,yyy,xxx";
    basic_istringstream<wchar_t> iss(records);
    BasicCSVParser<wchar_t, std::char_traits<wchar_t>, wstring, wstring, wstring> csv {iss};

    TripleWString expected1 = {L"aaa", L"bbb", L"ccc"};
    TripleWString expected2 = {L"zzz", L"yyy", L"xxx"};
    TripleWString t;

    csv >> t;
    ASSERT_EQ(t, expected1);
    csv >> t;
    ASSERT_EQ(t, expected2);
}

TEST(BasicCSVParser, WCharQuoteNewLineEnd) {
    wstring record = L"a,b,\"ccc,\n\n cc,\n c\ncc\"";
    basic_istringstream<wchar_t> iss(record);
    BasicCSVParser<wchar_t, std::char_traits<wchar_t>, wstring, wstring, wstring> csv {iss};

    TripleWString expected = {L"a",  L"b",  L"ccc,\n\n cc,\n c\ncc"};
    TripleWString t;
    csv >> t;
    ASSERT_EQ(t, expected);
}

int main(int argc, char** argv) {
    spdlog::set_level(spdlog::level::err);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}