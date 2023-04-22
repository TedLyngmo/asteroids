#pragma once

#include <SFML/System/Vector2.hpp>

#include <cmath>
#include <ostream>
#include <type_traits>

template<class T>
T lengthSquared(const sf::Vector2<T>& v) {
    return v.x * v.x + v.y * v.y;
}

template<class T>
T length(const sf::Vector2<T>& v) {
    return std::sqrt(lengthSquared(v));
}
template<class T>
std::ostream& operator<<(std::ostream& os, const sf::Vector2<T>& v) {
    return os << "sf::Vector2<T>{" << v.x << ',' << v.y << "} l:" << length(v) << " l^2:" << lengthSquared(v);
}

// findCentroid based on
// https://stackoverflow.com/a/33852627/7582247
template<class T>
sf::Vector2<T> findCentroid(const sf::Vector2<T>* pts, unsigned nPts) {
    sf::Vector2<T> off = pts[0];
    T twicearea = 0;
    T x = 0;
    T y = 0;
    sf::Vector2<T> p1, p2;
    T f;
    for (unsigned i = 0, j = nPts - 1; i < nPts; j = i++) {
        p1 = pts[i];
        p2 = pts[j];
        f = (p1.x - off.x) * (p2.y - off.y) - (p2.x - off.x) * (p1.y - off.y);
        twicearea += f;
        x += (p1.x + p2.x - 2 * off.x) * f;
        y += (p1.y + p2.y - 2 * off.y) * f;
    }

    f = twicearea * 3;

    return sf::Vector2<T>(x / f + off.x, y / f + off.y);
}
