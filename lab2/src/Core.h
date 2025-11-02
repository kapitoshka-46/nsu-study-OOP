#ifndef CORE_H
#define CORE_H

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

struct CellPos {
    int row;
    int col;
};

inline std::ostream& operator<<(std::ostream &lhs, const CellPos pos) {
    lhs << std::format("({},{})", pos.row, pos.col);
    return lhs;
}


class Field {
public:
    explicit Field(const std::string &s, int rows, int cols);

    int Rows() const;
    int Cols() const;

    bool IsAlive(CellPos pos);

    std::string ToString();
    int CountAliveAt(CellPos pos);
    void Set(CellPos pos, CellType state=CellType::kAlive);
    void Reset(CellPos pos);  // wrapper for Set(pos, CellType::kDead)
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
    : born_(std::move(born)),
    survival_(std::move(survival))
    {};

    const VecRules& GetBorn();
    const VecRules& GetSurvival();

    bool ShouldBorn(int alive);
    bool ShouldSurvival(int alive);
private:
    VecRules born_ {};
    VecRules survival_ {};
};

class Universe {

public:
    Universe(const Field& field, Rules rules) : field_(field), rules_(std::move(rules)) {};
    void Step();
    std::string ToString();

private:
    Field field_;
    Rules rules_;
};



#endif //CORE_H
