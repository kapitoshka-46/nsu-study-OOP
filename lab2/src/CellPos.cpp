#include "Core.h"

#include <format>
#include <sstream>
using namespace core;

CellPos core::parse_coordinates(std::string& line) {
    constexpr int ERROR_VAL = INT_MIN;
    std::istringstream iss {line};
    int col = ERROR_VAL;    // TODO: сделать operator>> который будет в CellPos записывать данные
    int row = ERROR_VAL;
    iss >> col >> row;

    return CellPos(row, col);
}

CellPos &CellPos::operator-=(const CellPos & other) {
    col -= other.col;
    row -= other.row;
    return *this;
}


bool CellPos::IsValid() const {
    return row != ERR_VAL and col != ERR_VAL;
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
