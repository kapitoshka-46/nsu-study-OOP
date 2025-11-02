#include "Core.h"

#include <algorithm>
#include <fstream>
#include <gtest/gtest.h>

using enum CellType;


Field::Field(const std::string &s, int rows, int cols) : rows_(rows), cols_(cols){
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

bool Rules::ShouldBorn(const int alive) {
    return std::ranges::any_of(born_, [alive](int x) {return x == alive;});
}

bool Rules::ShouldSurvival(int alive) {
    return std::ranges::any_of(survival_, [alive](int x) {return x == alive;});

}

// ---------------------------- UNIVERSE --------------------- //
void Universe::Step() {
    Field new_field = field_;

    for (int row = 0; row < field_.Rows(); row++) {
        for (int col = 0; col < field_.Cols(); col++) {
            CellPos pos = {row, col};
            if (pos.row == 8 and pos.col == 24) {
                std::cerr << 'r' << std::endl;
            }
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
