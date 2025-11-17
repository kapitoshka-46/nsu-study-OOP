#ifndef CORE_H
#define CORE_H

#include <format>
#include <vector>
#include <string>
#include <climits>

namespace core {
    static constexpr int ERR_VAL = INT_MIN;
    struct CellPos {
        int row = ERR_VAL;
        int col = ERR_VAL;

        CellPos &operator-=(const CellPos & other);
        CellPos &operator+=(const CellPos & other);

        [[nodiscard]] bool IsValid() const;
    };
    CellPos parse_coordinates_line(std::string& line);

    CellPos operator-(const CellPos &lhs, const CellPos &rhs);
    bool operator==(const CellPos &lhs, const CellPos &rhs);

    std::ostream& operator<<(std::ostream &lhs, CellPos pos);


    using VecRules = std::vector<char>;
    using Vec2D = std::vector<std::vector<bool>>;

    enum class CellType {
    kAlive,
    kDead,
};



    // ------------------------- Field ------------------------- //
    class Field {
        int rows_ {0};
        int cols_ {0};
        Vec2D matrix_ {};

    public:
        explicit Field(const std::string &s, int rows, int cols);
        explicit Field(int rows, int cols);
        ~Field();

        [[nodiscard]] int Rows() const;
        [[nodiscard]] int Cols() const;

        bool IsAlive(CellPos pos) const;

        std::string ToString() const;

        int CountAliveAt(CellPos pos);

        void Set(CellPos pos, CellType state=CellType::kAlive);

        void Reset(CellPos pos);  // wrapper for Set(pos, CellType::kDead)

        CellType GetCellState(CellPos pos) const;

        Field& operator=(const Field& other);

        void ResetAll();

        void Random();
    };
    std::ostream &operator<<(std::ostream & lhs, const Field & rhs);


    // -------------------------Rules------------------------- //

    class rules {
    public:
        explicit rules(VecRules born, VecRules survival);

        rules();
        ~rules();

        bool ShouldBorn(int alive) const;

        bool ShouldSurvival(int alive) const;

        void Reset();

        static VecRules GetDefaultBorn();

        static VecRules GetDefaultSurvival();

        static rules GetDefault();

        // TODO: make private
        VecRules born {3};

        VecRules survival {2, 3};

    };
    std::ostream &operator<<(std::ostream & lhs, const rules & rhs);

    // -------------------------Universe------------------------- //
    class universe {

    public:
        explicit universe(const Field& field, rules rules);
        explicit universe(int rows = 30, int cols = 40);
        ~universe();
        void Step();

        universe &Set(CellPos pos, CellType state = CellType::kAlive);
        void Reset(CellPos pos);

        [[nodiscard]] bool IsAlive(CellPos pos) const;

        [[nodiscard]] int Rows() const;
        [[nodiscard]] int Cols() const;

        void SetRules(const rules & rules);

        void SetName(const std::string & s);

        void ResetField();

        void GenerateRandom();

        void ResetUniverse();

        [[nodiscard]] const std::string &GetName() const;

        [[nodiscard]] const rules &GetRules() const;

        [[nodiscard]] int GetTickCount() const;

        [[nodiscard]] const Field &GetField() const;

    private:
        int ticks_ {0};
        std::string name_ {};
        Field field_;
        rules rules_;
    };
    std::ostream & operator<<(std::ostream &lhs, const universe &rhs);
}


#endif //CORE_H
