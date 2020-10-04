#ifndef POINT_H
#define POINT_H

template<typename T>
struct Point {
    T x, y, z;
    Point() : x(-1), y(-1), z(-1) {}
    Point(T a, T b, T c) : x(a), y(b), z(c) {}
};

#endif // POINT_H
