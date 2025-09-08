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
    int count {};
    std::ofstream out {};
public:
    Saver(std::string outPath);
    ~Saver();
    void changePath(std::string path);
    void saveMapAsCSV(std::map<std::string, unsigned> pairs);
};

Saver::Saver(std::string outPath) {
    std::cout << "File " << ++count << " opened." << '\n';
    out.open(outPath);
}
Saver::~Saver() {
    if (out.is_open()) {
        out.close();
    }
    std::cout << "File " << count-- << " closed." << '\n';
}

void Saver::saveMapAsCSV(std::map<std::string, unsigned> pairs) {
    for (auto [first, second]: pairs) {
        out << first << ',' << second << '\n';
    }
    std::cout << "Saved CSV to file " << count << ".\n";
}


int main(int argc, char** argv) {
    if (argc >= 3) {
        std::cout << argv[1] << "->" << argv[2];
    }
    else {
        // std::cout << "Ivalid argument. Usage: "<< argv[0] << " input.txt output.csv" <<'\n';
        // return 0;
    }

    std::string text = "MyStr omg it. is. us us is is is Mystr my. stt ?????  wtf!! bto";
    auto counter = WordCounter();
    auto& frequencyMap = counter.countWordsInStr(text);
    // for (auto &[word, freq] : frequencyMap) {
    //     std::cout << word << ' ' << freq << std::endl;
    // }
    
    auto outFile = Saver("out.csv");
    outFile.saveMapAsCSV(frequencyMap);
}