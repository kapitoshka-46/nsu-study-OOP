#ifndef POINT_H
#define POINT_H

#include <iostream>

namespace cfg {

    template<typename T>
    struct Point {
        T x,y;
    };

    template<typename T>
    std::ostream &operator<<(std::ostream & lhs, const Point<T> & pos) {
        lhs << '(' << pos.x << ", " << pos.y << ')';
        return lhs;
    }
} // cfg

#endif //POINT_H
