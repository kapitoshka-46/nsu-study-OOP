#include "Core.h"

#include <algorithm>
#include <fstream>
#include <gtest/gtest.h>

using enum CellType;


CellPos operator-(const CellPos &lhs, const CellPos &rhs) {
    CellPos lhs1 {lhs};
    return lhs1 -= rhs;
}

bool operator==(const CellPos &lhs, const CellPos &rhs) {
    return lhs.row == rhs.row and lhs.col == rhs.col;
}

Field::Field(const std::string &s, int rows, int cols) : rows_(rows), cols_(cols){
    if (s.length() != rows * cols) {
        throw std::invalid_argument("string must represent (rows x cols) matrix");
    }

    matrix_.resize(rows);
    for (auto& row : matrix_) {
        row.resize(cols);
    }
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            char c = s[y*cols + x];
            if (c == '-') {
                Set({y, x}, kDead);
            }
            if (c == '#') {
                Set({y, x}, kAlive);
            }
        }
    }
}

// inits zero field
Field::Field(int rows, int cols) : rows_(rows), cols_(cols){
    matrix_.resize(rows);
    for (auto &row : matrix_) {
        row.resize(cols);
    }
}

int Field::Rows() const {
    return rows_;
}

int Field::Cols() const {
    return cols_;
}

bool Field::IsAlive(CellPos pos) {
    if (GetCellState(pos) == kAlive) {
        return true;
    }
    return false;
}

std::string Field::ToString() {
    std::string s {};
    for (const auto& row : matrix_) {
        for (bool state : row) {
            s.push_back(state == true ? '@' : '-');
            s.push_back(' ');
        }
        s.push_back('\n');
    }
    return s;
}

int Field::CountAliveAt(CellPos pos) {
    int alive {0};
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            int row_mod = (pos.row + dy + Rows()) % Rows();
            int col_mod = (pos.col + dx + Cols()) % Cols();

            alive += matrix_.at(row_mod).at(col_mod);
        }
    }
    alive -= matrix_[pos.row][pos.col]; // cause shouldn't count cell itself
    return alive;

}

void Field::Set(CellPos pos, CellType state) {
    matrix_[pos.row][pos.col] = state == kAlive;
}

void Field::Reset(CellPos pos) {
    Set(pos, kDead);
}

CellType Field::GetCellState(CellPos pos) {
    if (pos.row < 0 or pos.row >= rows_) {
        throw std::out_of_range("row is out of range");
    }
    if (pos.col < 0 or pos.col >= cols_) {
        throw std::out_of_range("col is out of range");
    }

    if (matrix_[pos.row][pos.col] == true) {
        return kAlive;
    }
    return kDead;
}

Field& Field::operator=(const Field &other) = default;

bool Rules::ShouldBorn(const int alive) const {
    return std::ranges::any_of(born, [alive](int x) {return x == alive;});
}

bool Rules::ShouldSurvival(int alive) const {
    return std::ranges::any_of(survival, [alive](int x) {return x == alive;});
}

VecRules Rules::GetDefaultBorn() {
    return {3};
}

VecRules Rules::GetDefaultSurvival() {
    return {2, 3};
}

Universe::Universe(const Field &field, Rules rules) : field_(field), rules_(std::move(rules)) {}

Universe::Universe(int rows, int cols): field_(rows, cols) {}

// ---------------------------- UNIVERSE --------------------- //
void Universe::Step() {
    Field new_field = field_;

    for (int row = 0; row < field_.Rows(); row++) {
        for (int col = 0; col < field_.Cols(); col++) {
            CellPos pos = {row, col};
            int alive = field_.CountAliveAt(pos);
            if (field_.IsAlive(pos)) {
                if (not rules_.ShouldSurvival(alive)) {
                    new_field.Reset(pos);
                }
            }
            else if (rules_.ShouldBorn(alive)) {  // is not alive
                new_field.Set(pos);
            }
        }
    }

    field_ = new_field;
}

std::string Universe::ToString() {
    return field_.ToString();
}

void Universe::Set(CellPos pos, CellType state) {
    field_.Set(pos, state);
}

void Universe::Reset(CellPos pos) {
    field_.Reset(pos);
}

int Universe::Rows() const{
    return field_.Rows();
}

int Universe::Cols() const {
    return field_.Cols();
}

void Universe::SetRules(const Rules &rules) {
    if (rules.born.empty() or rules.survival.empty()) {
        throw std::logic_error("Cannot set empty rules!");
    }
    rules_ = rules;
}

VecRules parse_rules(std::string& s, char prefix) {
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


CellPos parse_coordinates(std::string& line) {
    std::istringstream iss {line};
    int col;
    int row;
    iss >> col >> row;

    return CellPos(row, col);
}

void LoadFromFile(std::ifstream &in, Universe& u) {
    if (!in) {
        throw std::invalid_argument("Cannot open a file");
    }
    // maybe call Universe::Clear() ?
    std::vector<CellPos> cells;
    CellPos max {INT_MIN, INT_MIN};
    CellPos min {INT_MAX, INT_MAX};

    int delta_row = 0;
    int delta_col = 0;

    std::string line;
    std::string name = "Unnamed";
    std::getline(in, line);

    Rules rules;
    if (line != "#Life 1.06") {
        std::cout << "Warning: Unknown file format.\n";
        std::cout << "Please, use \"Life 1.06\" file format\n";
    }
    bool is_name_set = false;
    while (std::getline(in, line)) {
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
                rules.born = parse_rules(line, 'B');
                rules.survival = parse_rules(line, 'S');
            }
        }
        else {
            CellPos cell = parse_coordinates(line);
            if (cell.row > max.row) {   max.row = cell.row;     }
            if (cell.col > max.col) {   max.col = cell.col;     }
            if (cell.row < min.row) {   min.row = cell.row;     }
            if (cell.col < min.col) {   min.col = cell.col;     }

            delta_row = max.row - min.row;
            delta_col = max.col - min.col;
            if (delta_row > kMaxRows) {
                std::cout << "Error: Figure if file needs more rows then max available rows\n";
                throw std::logic_error("Not enough rows");
            }
            if (delta_col > kMaxCols) {
                std::cout << "Error: Figure in file needs more columns then max available columns\n";
                throw std::logic_error("Not enough cols");
            }
            if (delta_row < 0 || delta_col < 0) {
                throw std::logic_error("min > max ?");
            }

            cells.push_back(cell);
        }


    }
    if (rules.born.empty()) {
        std::cout << "Warning: No born rules found. Using default: Born if 2 or 3 cells alive.\n";
        rules.born = Rules::GetDefaultBorn();
    }
    if (rules.survival.empty()) {
        std::cout << "Warning: No survival rules found. Using default: Survive if 3 cell alive.\n";
        rules.survival = Rules::GetDefaultSurvival();
    }


    CellPos origin {u.Rows() / 2 - delta_row / 2, u.Cols() / 2 - delta_col / 2};

    for (auto &cell : cells) {
        cell += origin;
        u.Set(cell);
    }

    u.SetRules(rules);
}