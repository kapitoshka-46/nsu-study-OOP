#ifndef CORE_H
#define CORE_H

#include "CellPos.h"
#include <format>
#include <utility>
#include <vector>
#include <string>

namespace game_core {
    using namespace cell_pos;

    using VecRules = std::vector<char>;
    using Vec2D = std::vector<std::vector<bool>>;

    enum class CellType {
    kAlive,
    kDead,
};



    // ------------------------- Field ------------------------- //
    class Field {
    private:
        int rows_ {0};
        int cols_ {0};
        Vec2D matrix_ {};

    public:
        explicit Field(const std::string &s, int rows, int cols);
        explicit Field(int rows, int cols);

        [[nodiscard]] int Rows() const;
        [[nodiscard]] int Cols() const;

        bool IsAlive(CellPos pos);

        std::string ToString();

        int CountAliveAt(CellPos pos);

        void Set(CellPos pos, CellType state=CellType::kAlive);

        void Reset(CellPos pos);  // wrapper for Set(pos, CellType::kDead)

        CellType GetCellState(CellPos pos);

        Field& operator=(const Field& other);


    };


    // -------------------------Rules------------------------- //
    class Rules {
    public:
        explicit Rules(VecRules born, VecRules survival);
        Rules();

        bool ShouldBorn(int alive) const;
        bool ShouldSurvival(int alive) const;
        static VecRules GetDefaultBorn();
        static VecRules GetDefaultSurvival();

        VecRules born {3};
        VecRules survival {2, 3};
    };


    // -------------------------Universe------------------------- //
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




}


#endif //CORE_H
