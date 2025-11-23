#ifndef CORE_H
#define CORE_H
// ------------ Constructors ------------
// ------------- Destructor -------------
// -------------- Getters ---------------
// -------------- Setters ---------------
// -------------- Actions ---------------
#include <vector>
#include <string>
#include <climits>

namespace core {
    struct CellPos {
        int row = 0;
        int col = 0;

        CellPos &operator-=(const CellPos & other);
        CellPos &operator+=(const CellPos & other);

        CellPos(int row, int col);
        CellPos();
    };

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

    public:
        // --------------- Constructors ---------------
        explicit Field(const std::string &s, int rows, int cols);
        explicit Field(int rows, int cols);

        // --------------- Destructor -----------------
        ~Field();

        // --------------- Getters ---------------
        [[nodiscard]] int Rows() const;

        [[nodiscard]] int Cols() const;

        [[nodiscard]] const Vec2D &GetMatrix() const;

        [[nodiscard]] bool IsAlive(CellPos pos) const;

        [[nodiscard]] CellType GetCellState(CellPos pos) const;


        // --------------- Actions ---------------
        int CountAliveAt(CellPos pos);

        void GenerateRandomField();

        // --------------- Setters ---------------
        void Set(CellPos pos, CellType state=CellType::kAlive);

        void Reset(CellPos pos);  // alias for Set(pos, CellType::kDead)

        Field& operator=(const Field& other);   // TODO:  operator=(Field && other)

        void ResetAll();

    private:
        int rows_ {0};
        int cols_ {0};
        Vec2D matrix_ {};
    };
    std::ostream &operator<<(std::ostream & lhs, const Field & rhs);


    // -------------------------Rules------------------------- //
    class Rules {
    public:
        // ------------ Constructors ------------
        explicit Rules(VecRules born, VecRules survival);

        Rules();

        Rules(const Rules &other);

        // ------------- Destructor -------------
        ~Rules();

        // -------------- Getters ---------------
        [[nodiscard]] const VecRules &GetBorn() const;

        [[nodiscard]] const VecRules &GetSurvival() const;

        static VecRules GetDefaultBorn();

        static VecRules GetDefaultSurvival();

        static Rules GetDefault();

        // -------------- Setters ---------------
        void Reset();

        void SetDefaultBorn();

        void SetDefaultSurvival();

        void SetRules(VecRules born, VecRules survival);

        // -------------- Actions ---------------
        [[nodiscard]] bool ShouldBorn(int alive) const;

        [[nodiscard]] bool ShouldSurvival(int alive) const;


        Rules &operator=(const Rules &rules);

        Rules &operator=(Rules &&rules) noexcept ;

    private:
        VecRules born {3};

        VecRules survival {2, 3};

    };
    std::ostream &operator<<(std::ostream & lhs, const Rules & rhs);

    // -------------------------Universe------------------------- //
    class Universe {

    public:
        // ---------------- Constructors -------------------
        explicit Universe(const Field& field, Rules rules);

        explicit Universe(int rows = 30, int cols = 40);

        // ----------------- Destructor ----------------------
        ~Universe();


        // ------------------ Getters -------------------------
        [[nodiscard]] const std::string &GetName() const;

        [[nodiscard]] const Rules &GetRules() const;

        [[nodiscard]] int GetTickCount() const;

        [[nodiscard]] const Field &GetField() const;

        [[nodiscard]] bool IsAlive(CellPos pos) const;

        [[nodiscard]] int Rows() const;

        [[nodiscard]] int Cols() const;


        // --------------------- Setters ----------------------
        Universe &Set(CellPos pos, CellType state = CellType::kAlive);

        void Reset(CellPos pos);

        void SetRules(const Rules & rules);

        void SetName(const std::string & s);

        void ResetField();

        void ResetUniverse();

        // -------------------- Actions ------------------------
        void Step();

        void GenerateRandom();



    private:
        int ticks_ {0};
        std::string name_ {};
        Field field_;
        Rules rules_;
    };
    std::ostream & operator<<(std::ostream &lhs, const Universe &rhs);
}


#endif //CORE_H
