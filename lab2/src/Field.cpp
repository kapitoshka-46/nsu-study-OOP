#include <bitset>

#include "Core.h"
#include <iostream>
#include <random>
#include <boost/dynamic_bitset/dynamic_bitset.hpp>

using namespace core;
using enum CellType;


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
    if (rows <= 0 or cols <= 0) {
        throw std::invalid_argument("field: rows <= 0 or cols <= 0");
    }
    matrix_.resize(rows);
    for (auto &row : matrix_) {
        row.resize(cols);
    }
}

Field::~Field() {
    std::cerr << "field dest\n";
}

int Field::Rows() const {
    return rows_;
}

int Field::Cols() const {
    return cols_;
}

bool Field::IsAlive(CellPos pos) const {
    if (GetCellState(pos) == kAlive) {
        return true;
    }
    return false;
}

std::string Field::ToString() const {
    std::string s {};
    for (const auto& row : matrix_) {
        for (bool state : row) {
            s.push_back(state == true ? '0' : '.');
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

CellType Field::GetCellState(CellPos pos) const {
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

void Field::ResetAll() {
    for (auto &row : matrix_) {
        auto sz = row.size();
        row.clear();
        row.resize(sz);
    }
}

void Field::Random() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution dist(0, 1); // Диапазон

    for (auto &row : matrix_) {
        for (int j = 0; j < Cols(); j++) {
            row[j] = dist(gen);
        }
    }
}


std::ostream &core::operator<<(std::ostream &lhs, const Field &rhs) {
    lhs << rhs.ToString(); // TODO: сделать нотрмально
    return lhs;
}
