#include "Core.h"

#include <format>
#include <sstream>
using namespace core;

CellPos::CellPos(int row, int col) : row(row), col(col) {}

CellPos::CellPos() = default;

CellPos &CellPos::operator-=(const CellPos & other) {
    col -= other.col;
    row -= other.row;
    return *this;
}

CellPos & CellPos::operator+=(const CellPos &other) {
    col += other.col;
    row += other.row;
    return *this;
}

CellPos core::operator-(const CellPos &lhs, const CellPos &rhs) {
    CellPos lhs1 {lhs};
    return lhs1 -= rhs;
}

bool core::operator==(const CellPos &lhs, const CellPos &rhs) {
    return lhs.row == rhs.row and lhs.col == rhs.col;
}

std::ostream & core::operator<<(std::ostream &lhs, const CellPos pos) {
    lhs << std::format("({},{})", pos.row, pos.col);
    return lhs;
}
