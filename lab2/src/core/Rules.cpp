#include "Core.h"
#include <algorithm>
#include <ostream>

using namespace core;

std::ostream& core::operator<<(std::ostream & lhs, const Rules & rhs) {
    lhs << "Born: ";
    for (auto rule : rhs.GetBorn()) {
        lhs << static_cast<int>(rule);
    }
    lhs << " | Survive: ";
    for (auto rule : rhs.GetSurvival()) {
        lhs << static_cast<int>(rule);
    }
    return lhs;
}

bool Rules::ShouldBorn(const int alive) const {
    return std::ranges::any_of(born, [alive](int x) {return x == alive;});
}

bool Rules::ShouldSurvival(int alive) const {
    return std::ranges::any_of(survival, [alive](int x) {return x == alive;});
}

void Rules::Reset() {
    born.clear();
    survival.clear();
}

VecRules Rules::GetDefaultBorn(){
    return {3};
}

VecRules Rules::GetDefaultSurvival(){
    return {2, 3};
}

Rules Rules::GetDefault() {
    return Rules{GetDefaultBorn(), GetDefaultSurvival()};
}

void Rules::SetDefaultBorn() {
    born = GetDefaultBorn();
}

void Rules::SetDefaultSurvival() {
    survival = GetDefaultSurvival();
}

const VecRules & Rules::GetBorn() const {
    return born;
}

const VecRules & Rules::GetSurvival() const {
    return survival;
}

void Rules::SetRules(VecRules born, VecRules survival) {
    this->born = std::move(born);
    this->survival = std::move(survival);
}

Rules & Rules::operator=(const Rules &rules) = default;

Rules & Rules::operator=(Rules &&rules)  noexcept {
    this->born = rules.born;
    this->survival = rules.survival;
    return *this;
}

Rules::Rules(VecRules born, VecRules survival): born(std::move(born)),
                                                survival(std::move(survival)) {}

Rules::Rules() = default;

Rules::Rules(const Rules &other) = default;

Rules::~Rules() = default;
