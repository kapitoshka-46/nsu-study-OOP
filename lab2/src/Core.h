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

constexpr int kMaxRows = 100;
constexpr int kMaxCols = 300;


inline std::ostream& operator<<(std::ostream &lhs, const CellPos pos) {
    lhs << std::format("({},{})", pos.row, pos.col);
    return lhs;
}

 
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

    VecRules born {};
    VecRules survival {};
};

struct LifeFile_1_06 {
    std::string name {};
    std::vector<CellPos> positions {};
    VecRules born {};
    VecRules survival {};
};
class Universe {

public:
    Universe(const Field& field, Rules rules) : field_(field), rules_(std::move(rules)) {};
    void Step();
    void Erase();                       // Erase the field and Rules
    void Resize(int rows, int cols);
    std::string ToString();
    void LoadFromFile(std::ifstream &in);

    void Set(CellPos pos, CellType state=CellType::kAlive);
    void Reset(CellPos);

    int Rows();
    int Cols();
private:
    std::string name {};
    Field field_;
    Rules rules_;
};



#endif //CORE_H
