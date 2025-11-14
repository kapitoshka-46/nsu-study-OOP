#include "Core.h"
#include <iostream>

using namespace core;


Universe::Universe(const Field &field, Rules rules) : field_(field), rules_(std::move(rules)){}

Universe::Universe(int rows, int cols): field_(rows, cols) {}

Universe::~Universe() {
    std::cerr << "Universe dest\n";
}

void Universe::Step() {
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

void Universe::Set(CellPos pos, CellType state) {
    field_.Set(pos, state);
}

void Universe::Reset(CellPos pos) {
    field_.Reset(pos);
}

bool Universe::IsAlive(CellPos pos) const {
    return field_.IsAlive(pos);
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
    name_ = s;
}

void Universe::ResetField() {
    field_.ResetAll();
}

void Universe::GenerateRandom() {
    this->ResetUniverse();
    name_ = "Random";
    rules_ = Rules::GetDefault();
    field_.Random();
}

void Universe::ResetUniverse() {
    ResetField();
    rules_.Reset();
    name_ = "Unnamed";
    ticks_ = 0;
}

const std::string & Universe::GetName() const {
    return name_;
}

const Rules &Universe::GetRules() const {
    return rules_;
}

int Universe::GetTickCount() const {
    return ticks_;
}

const Field & Universe::GetField() const {
    return field_;
}



std::ostream & core::operator<<(std::ostream &lhs, const Universe &rhs) {
    lhs << "\nName: " << rhs.GetName();
    lhs << "\nRules: " << rhs.GetRules();
    lhs << "\nTicks: " << rhs.GetTickCount();
    lhs << "\nField:\n" << rhs.GetField();
    return lhs;
}
