#ifndef CORE_H
#define CORE_H

#include <climits>
#include <format>
#include <utility>
#include <vector>
#include <string>
#include <fstream>

using VecRules = std::vector<char>;
using Vec2D = std::vector<std::vector<bool>>;

enum class CellType {
    kAlive,
    kDead,
};

static constexpr int NAN = INT_MIN;
struct CellPos {

    int row = NAN;
    int col = NAN;

    CellPos &operator-=(const CellPos & other) {
        col -= other.col;
        row -= other.row;
        return *this;
    }

    CellPos &operator+=(const CellPos & other) {
        col += other.col;
        row += other.row;
        return *this;
    }
    bool IsValid() const {
        return row != NAN and col != NAN;
    }
};
CellPos operator-(const CellPos &lhs, const CellPos &rhs);
bool operator==(const CellPos &lhs, const CellPos &rhs);

constexpr int kMaxRows = 100;
constexpr int kMaxCols = 300;


inline std::ostream& operator<<(std::ostream &lhs, const CellPos pos) {
    lhs << std::format("({},{})", pos.row, pos.col);
    return lhs;
}

void move_to_origin(std::vector<CellPos> &cells, CellPos origin);
CellPos parse_coordinates(std::string& line);

class Field {
public:
    explicit Field(const std::string &s, int rows, int cols);
    explicit Field(int rows, int cols);

    int Rows() const;
    int Cols() const;

    bool IsAlive(CellPos pos);

    std::string ToString();

    int CountAliveAt(CellPos pos);

    void Set(CellPos pos, CellType state=CellType::kAlive);

    void Reset(CellPos pos);  // wrapper for Set(pos, CellType::kDead)

    void Resize(int rows, int cols);

    CellType GetCellState(CellPos pos);

    Field& operator=(const Field& other);

private:
    int rows_ {0};
    int cols_ {0};
    Vec2D matrix_ {};
};
// ------------------ END FIELD --------------//

// ----------------- RULES ----------------- //
class Rules {
public:
    explicit Rules(VecRules  born, VecRules survival)
    : born(std::move(born)),
    survival(std::move(survival))
    {};
    Rules() = default;

    bool ShouldBorn(int alive) const;
    bool ShouldSurvival(int alive) const;
    static VecRules GetDefaultBorn();
    static VecRules GetDefaultSurvival();

    VecRules born {3};
    VecRules survival {2, 3};
};



class Universe {

public:
    Universe(const Field& field, Rules rules);;
    Universe(int rows, int cols);
    void Step();
    std::string ToString();

    void Set(CellPos pos, CellType state=CellType::kAlive);
    void Reset(CellPos pos);

    int Rows() const;
    int Cols() const;

    void SetRules(const Rules & rules);

    void SetName(const std::string & s);

private:
    std::string name {};
    Field field_;
    Rules rules_;
};

void LoadFromFile(std::ifstream &in, Universe& u);

#endif //CORE_H
