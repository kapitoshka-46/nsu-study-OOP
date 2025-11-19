#include "file_handler.h"

#include <fstream>

#include "../core/Core.h"
#include <iostream>
#include <sstream>

using namespace core;

CellPos parse_coordinates_line(std::string& line);

VecRules parse_rules(std::string const &s, char prefix);



void file_handler::LoadFromFile(std::ifstream &in, universe& u /*, callback print function */) {
    if (!in) {
        throw std::invalid_argument("Cannot open a file");
    }
    u.ResetUniverse();

    std::vector<CellPos> cells;
    CellPos max {INT_MIN, INT_MIN};
    CellPos min {INT_MAX, INT_MAX};

    int delta_row = 0;
    int delta_col = 0;

    std::string line;
    std::string name = "Unnamed";
    std::getline(in, line);

    rules rules {{}, {}};
    if (line != "#Life 1.06") {
        std::cout << "Invalid line: \"" << line << "\"\n";
        std::cout << "Warning: Unknown file format.\n";
        std::cout << "Please, use Life 1.06 file format\n";
        std::cout << "Loading stopped\n";
        return;
    }
    bool is_name_set = false;
    bool is_rools_set = false;
    while (std::getline(in, line)) {
        if (line.empty()) {
            std::cout << "Skip empty line\n";
            continue;
        }
        // #N Name of universe
        if (line[0] == '#') {
            if (line.substr(0, 3) == "#N ") {   // name
                if (is_name_set) {
                    std::cout <<"Warning: founded more then one names. Using first one\n";
                    continue;
                }
                name = line.substr(3);
                is_name_set = true;
            }
            // #R Bxyz/Sabc
            if (line.substr(0, 3) == "#R ") {    // rules
                if (is_rools_set) {
                    std::cout << "[Warning] founded more then one rules. Using first one\n";
                    continue;
                }
                is_rools_set = true;
                rules.born = parse_rules(line, 'B');
                rules.survival = parse_rules(line, 'S');
            }
        }
        else {
            CellPos cell = parse_coordinates_line(line);
            if (!cell.IsValid()) {
                std::cout << "[Warning] Invalid coordinate line: \"" << line << "\"\n";
                continue;
            }
            if (cell.row > max.row) {   max.row = cell.row;     }
            if (cell.col > max.col) {   max.col = cell.col;     }
            if (cell.row < min.row) {   min.row = cell.row;     }
            if (cell.col < min.col) {   min.col = cell.col;     }

            delta_row = max.row - min.row;
            delta_col = max.col - min.col;
            cells.push_back(cell);
        }
    }

    if (delta_row > u.Rows()) {
        throw std::logic_error("[Error] Figure in file needs more rows then max available rows");
    }
    if (delta_col > u.Cols()) {
        throw std::logic_error("[Error] Figure in file needs more columns then max available columns");
    }
    if (delta_row < 0 || delta_col < 0) {
        throw std::logic_error("[Error] Unexpected error. min > max ?");
    }
    if (rules.born.empty()) {
        std::cout << "[Warning] No born rules found. Using default: Born if 3 cells alive.\n";
        rules.born = rules::GetDefaultBorn();
    }
    if (rules.survival.empty()) {
        std::cout << "[Warning] No survival rules found. Using default: Survive if 2 or 3 cells alive.\n";
        rules.survival = rules::GetDefaultSurvival();
    }


    CellPos origin {u.Rows() / 2 - delta_row / 2, u.Cols() / 2 - delta_col / 2};
    for (auto &cell : cells) {
        cell += origin - min;
        u.Set(cell);
    }

    u.SetRules(rules);
    u.SetName(name);
    std::cout << "Name set as: " << name << '\n';
    std::cout << "Rules set as: " << rules << '\n';
    std::cout << "Loading complete!" << std::endl;;
}

void file_handler::SaveToFile(std::ofstream &out, const core::universe &u) {
    out << "#Life 1.06\n";
    out << "#N " + u.GetName() + '\n';
    const rules &rules = u.GetRules();

    // save rules
    out << "#R B";
    for (auto rule : rules.born) {
        out << static_cast<int>(rule);
    }
    out << "/S";
    for (auto rule: rules.survival) {
        out << static_cast<int>(rule);
    }
    out << '\n';

    // save coordinates
    for (int row = 0; row < u.Rows(); row++) {
        for (int col = 0; col < u.Cols(); col++) {
            if (u.IsAlive({row, col})) {
                out << col << ' ' << row << '\n';
            }
        }
    }
}

CellPos parse_coordinates_line(std::string& line) {
    constexpr int ERROR_VAL = INT_MIN;
    std::istringstream iss {line};
    int col = ERROR_VAL;    // TODO: сделать operator>> который будет в CellPos записывать данные
    int row = ERROR_VAL;
    iss >> col >> row;

    return CellPos(row, col);
}

VecRules parse_rules(std::string const &s, char prefix) {
    VecRules rules {};
    size_t pos = s.find_first_of(prefix);
    if (pos != std::string::npos) {
        pos += 1;
        while (std::isdigit(s[pos])) {
            rules.push_back(s[pos] - '0');
            pos++;
        }
    }
    return rules;
}