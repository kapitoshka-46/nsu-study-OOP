#ifndef CELLPOS_H
#define CELLPOS_H

#include <string>
#include <climits>
namespace cell_pos {

    static constexpr int NAN = INT_MIN;
    struct CellPos {
        int row = NAN;
        int col = NAN;

        CellPos &operator-=(const CellPos & other);
        CellPos &operator+=(const CellPos & other);

        bool IsValid() const;
    };
    CellPos parse_coordinates(std::string& line);

    CellPos operator-(const CellPos &lhs, const CellPos &rhs);
    bool operator==(const CellPos &lhs, const CellPos &rhs);

    std::ostream& operator<<(std::ostream &lhs, CellPos pos);
}


#endif //CELLPOS_H
