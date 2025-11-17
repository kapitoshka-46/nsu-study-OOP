#include "Core.h"
#include <iostream>

using namespace core;


universe::universe(const Field &field, rules rules) : field_(field), rules_(std::move(rules)){}

universe::universe(int rows, int cols): field_(rows, cols) {
    if (rows <= 0 or cols <= 0) {
        throw std::invalid_argument("rows or cols <= 0");
    }
}

universe::~universe() {}

void universe::Step() {
    ticks_++;
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

universe &universe::Set(CellPos pos, CellType state) {
    field_.Set(pos, state);
    return *this;
}

void universe::Reset(CellPos pos) {
    field_.Reset(pos);
}

bool universe::IsAlive(CellPos pos) const {
    return field_.IsAlive(pos);
}

int universe::Rows() const{
    return field_.Rows();
}

int universe::Cols() const {
    return field_.Cols();
}

void universe::SetRules(const rules &rules) {
    if (rules.born.empty() or rules.survival.empty()) {
        throw std::logic_error("Cannot set empty rules!");
    }
    rules_ = rules;
}

void universe::SetName(const std::string &s) {
    if (s.empty()) {
        throw std::invalid_argument("Cannot set empty name");
    }
    name_ = s;
}

void universe::ResetField() {
    field_.ResetAll();
}

void universe::GenerateRandom() {
    this->ResetUniverse();
    name_ = "Random";
    rules_ = rules::GetDefault();
    field_.Random();
}

void universe::ResetUniverse() {
    ResetField();
    rules_.Reset();
    name_ = "Unnamed";
    ticks_ = 0;
}

const std::string & universe::GetName() const {
    return name_;
}

const rules &universe::GetRules() const {
    return rules_;
}

int universe::GetTickCount() const {
    return ticks_;
}

const Field & universe::GetField() const {
    return field_;
}



std::ostream & core::operator<<(std::ostream &lhs, const universe &rhs) {
    lhs << "\nName: " << rhs.GetName();
    lhs << "\nRules: " << rhs.GetRules();
    lhs << "\nTicks: " << rhs.GetTickCount();
    lhs << "\nField:\n" << rhs.GetField();
    return lhs;
}
