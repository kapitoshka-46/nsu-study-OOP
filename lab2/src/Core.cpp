#include "Core.h"

using enum CellType;


Field::Field(const std::string &s, int rows, int cols) {
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

void Field::Set(CellPos pos, CellType state) {
    matrix_[pos.row][pos.col] = state == kAlive;
}
