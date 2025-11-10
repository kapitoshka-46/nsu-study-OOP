#include "Core.h"
using namespace game_core;


Universe::Universe(const Field &field, Rules rules) : field_(field), rules_(std::move(rules)) {}

Universe::Universe(int rows, int cols): field_(rows, cols) {}

void Universe::Step() {
    Field new_field = field_;

    for (int row = 0; row < field_.Rows(); row++) {
        for (int col = 0; col < field_.Cols(); col++) {
            CellPos pos = {row, col};
            int alive = field_.CountAliveAt(pos);
            if (field_.IsAlive(pos)) {
                if (not rules_.ShouldSurvival(alive)) {
                    new_field.Reset(pos);
                }
            }
            else if (rules_.ShouldBorn(alive)) {  // is not alive
                new_field.Set(pos);
            }
        }
    }

    field_ = new_field;
}
std::string Universe::ToString() {
    return field_.ToString();
}

void Universe::Set(CellPos pos, CellType state) {
    field_.Set(pos, state);
}

void Universe::Reset(CellPos pos) {
    field_.Reset(pos);
}

int Universe::Rows() const{
    return field_.Rows();
}

int Universe::Cols() const {
    return field_.Cols();
}

void Universe::SetRules(const Rules &rules) {
    if (rules.born.empty() or rules.survival.empty()) {
        throw std::logic_error("Cannot set empty rules!");
    }
    rules_ = rules;
}

void Universe::SetName(const std::string &s) {
    if (s.empty()) {
        throw std::invalid_argument("Cannot set empty name");
    }
    name = s;
}