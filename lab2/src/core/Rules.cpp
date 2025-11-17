#include "Core.h"
#include <algorithm>
#include <ostream>
#include <iostream>

using namespace core;

std::ostream& core::operator<<(std::ostream & lhs, const rules & rhs) {
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

bool rules::ShouldBorn(const int alive) const {
    return std::ranges::any_of(born, [alive](int x) {return x == alive;});
}

bool rules::ShouldSurvival(int alive) const {
    return std::ranges::any_of(survival, [alive](int x) {return x == alive;});
}

void rules::Reset() {
    born.clear();
    survival.clear();
}

VecRules rules::GetDefaultBorn(){
    return {3};
}

VecRules rules::GetDefaultSurvival(){
    return {2, 3};
}

rules rules::GetDefault() {
    return rules{GetDefaultBorn(), GetDefaultSurvival()};
}

rules::rules(VecRules born, VecRules survival): born(std::move(born)),
                                                survival(std::move(survival)) {}

rules::rules() = default;

rules::~rules() {}
