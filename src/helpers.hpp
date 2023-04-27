#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include <cmath>
#include <iterator>
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
T toRadians(const sf::Vector2<T>& v) {
    return std::atan2(v.y, v.x);
}

template<class T>
std::ostream& operator<<(std::ostream& os, const sf::Vector2<T>& v) {
    return os << "sf::Vector2<T>{" << v.x << ',' << v.y << "} length:" << length(v) << " length^2:" << lengthSquared(v);
}
template<class T>
std::ostream& operator<<(std::ostream& os, const sf::Rect<T>& r) {
    return os << "sf::Rect<T>{" << r.left << ',' << r.top << ',' << r.width << ',' << r.height << '}';
}

// findCentroid based on
// https://stackoverflow.com/a/33852627/7582247
template<class Iter, class V2 = typename std::iterator_traits<Iter>::value_type>
V2 findCentroid(Iter first, Iter last) {
    if(first == last) return V2{};                   // {0,0} or throw or leave UB

    if(auto next = std::next(first); next == last) { // a single point
        return *first;
    } else if(std::next(next) == last) {             // a line
        const auto& [x1, y1] = *first;
        const auto& [x2, y2] = *next;
        return V2{(x1 + x2) / 2, (y1 + y2) / 2};     // midpoint of the line
    }

    // three or more points:
    const auto& [offx, offy] = *first;
    using T1 = std::remove_cvref_t<decltype(offx)>;
    using T2 = std::remove_cvref_t<decltype(offy)>;
    T1 x{};
    T2 y{};
    decltype((x * y) - (x * y)) twicearea{}, f;

    for(auto prev = std::prev(last); first != last; prev = first++) {
        const auto& [p1x, p1y] = *first;
        const auto& [p2x, p2y] = *prev;
        f = (p1x - offx) * (p2y - offy) - (p2x - offx) * (p1y - offy);
        twicearea += f;
        x += (p1x + p2x - 2 * offx) * f;
        y += (p1y + p2y - 2 * offy) * f;
    }

    f = twicearea * 3;

    return V2{static_cast<T1>(x / f + offx), static_cast<T2>(y / f + offy)};
}
