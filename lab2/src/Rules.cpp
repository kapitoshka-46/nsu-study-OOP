#include "Core.h"
#include <algorithm>
#include <ostream>
#include <iostream>

using namespace core;

std::ostream& core::operator<<(std::ostream & lhs, const Rules & rhs) {
    lhs << "Born: ";
    for (auto rule : rhs.born) {
        lhs << static_cast<int>(rule);
    }
    lhs << " | Survive: ";
    for (auto rule : rhs.survival) {
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

Rules::Rules(VecRules born, VecRules survival): born(std::move(born)),
                                                survival(std::move(survival)) {}

Rules::Rules() = default;

Rules::~Rules() {
    std::cerr << "Rules dest\n";
}
