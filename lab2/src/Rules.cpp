#include "Core.h"
#include <algorithm>

using namespace game_core;

bool Rules::ShouldBorn(const int alive) const {
    return std::ranges::any_of(born, [alive](int x) {return x == alive;});
}

bool Rules::ShouldSurvival(int alive) const {
    return std::ranges::any_of(survival, [alive](int x) {return x == alive;});
}

VecRules Rules::GetDefaultBorn(){
    return {3};
}

VecRules Rules::GetDefaultSurvival(){
    return {2, 3};
}