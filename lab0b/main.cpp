#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <boost/algorithm/string.hpp>

class InputHandler {
private:
    std::ifstream &in;
    std::string *buffer = nullptr;
public:
    InputHandler(std::ifstream &in) :in(in) {
        buffer = new std::string();
    }
    ~InputHandler() {
        delete buffer;
    }

    std::string& getNewText() {
        std::getline(in, *buffer);
        return *buffer;
    }
};


class WordCounter {
private:
    std::map<std::string, unsigned> frequency;

    void makeWordsFromStr_(std::string text) {}
public:

    WordCounter() {}
    ~WordCounter() {}

    const std::map<std::string, unsigned> & countWordsInStr(std::string text) {
        // Find a word
        std::string separators = " ;:,./\'\"<>!?@()[]{}=+-\\|\n";
        size_t start = text.find_first_not_of(separators);
        while (start != std::string::npos) {
            size_t end = text.find_first_of(separators, start + 1);
            if (end == std::string::npos) {
                end = text.length() - 1;
            }
            // the word is founded
            auto word = text.substr(start, end - start);
            boost::algorithm::to_lower(word);
            frequency[word] += 1;
            start = text.find_first_not_of(separators, end + 1);
        }
        return frequency;
    }
};

class Saver {
private:
    std::ofstream & out;
public:
    Saver(std::ofstream & out) : out(out) {}
    ~Saver() {};

    bool SaveMapToCSV();   // сохраняет мапу в csv  файл
};

int main(int argc, char** argv) {
    if (argc >= 3) {
        std::cout << argv[1] << "->" << argv[2];
    }

    std::string text = "MyStr omg it. is. us us is is is Mystr my. stt ?????  wtf!! bto";
    auto counter = WordCounter();
    auto& map = counter.countWordsInStr(text);
    for (auto &[word, freq] : map) {
        std::cout << word << ' ' << freq << std::endl;
    }
}