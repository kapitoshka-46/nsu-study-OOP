#ifndef CORE_H
#define CORE_H

#include <vector>
#include <string>

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

class Rules {
public:
    const VecRules& GetBorn();
    const VecRules& GetSurvival();
private:
    VecRules born {};
    VecRules survival {};


};

class Field {
public:
    explicit Field(const std::string &s, int rows, int cols);

    int SizeRows();
    int SizeCols();

    std::string ToString();
    void Set(CellPos pos, CellType state=CellType::kAlive);
    void Reset(CellPos pos);  // wrapper for Set(pos, CellType::kDead)
    CellType GetCellState(CellPos pos);

    Field& operator=(const Field& other);

private:
    int rows_ {0};
    int cols_ {0};
    Vec2D matrix_ {};
};
class Universe {

public:
    void Step();
    std::string ToString();

private:
    Field field;
    Rules rules;
};



#endif //CORE_H
