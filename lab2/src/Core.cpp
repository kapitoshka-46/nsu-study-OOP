#include "Core.h"
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

int Field::SizeRows() {
    return rows_;
}

int Field::SizeCols() {
    return cols_;
}

std::string Field::ToString() {
    std::string s {};
    for (const auto& row : matrix_) {
        for (bool state : row) {
            s.push_back(state == true ? '#' : '-');
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
            int row_mod = (pos.row + dy + SizeRows()) % SizeRows();
            int col_mod = (pos.row + dx + SizeRows()) % SizeRows();

            alive += matrix_.at(row_mod).at(col_mod);
        }
    }
    alive -= matrix_[pos.row][pos.col]; // cause shouldn't count cell itself
    return alive;

}

void Field::Set(CellPos pos, CellType state) {
    matrix_[pos.row][pos.col] = state == kAlive;
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
// ---------------------------- UNIVERSE --------------------- //
void Universe::Step() {

}
