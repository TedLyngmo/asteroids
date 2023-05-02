#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iterator>
#include <optional>
#include <ostream>
#include <type_traits>

template<class T>
struct BoundingRect {
    BoundingRect() = default;
    explicit constexpr BoundingRect(const sf::Rect<T>&r) : left(r.left), top(r.top), right(r.left + r.width), bottom(r.top+r.height) {}
    explicit constexpr BoundingRect(const sf::Vector2<T>& center, const sf::Vector2<T>& size) : BoundingRect(sf::Rect<T>(sf::Vector2<T>(center.x - size.x / 2.f, center.y - size.y / 2.f), sf::Vector2<T>(size.x, size.y))) {}
    explicit constexpr BoundingRect(const sf::View& view) : BoundingRect(view.getCenter(), view.getSize()) {}

    BoundingRect& operator=(const sf::Rect<T>&r) {
        *this = BoundingRect(r);
        return *this;
    }
    BoundingRect& operator=(const sf::View& v) {
        *this = BoundingRect(v);
        return *this;
    }

    T left;
    T top;
    T right;
    T bottom;
};

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

template <typename T>
constexpr T cross(const sf::Vector2<T>& lhs, const sf::Vector2<T>& rhs) {
    return lhs.x * rhs.y - lhs.y * rhs.x;
}

inline sf::Vector2f getCentroid(const sf::Shape& shape) {
    const auto count = shape.getPointCount();

    switch (count) {
        case 0:
            assert(false);
            return {};
        case 1:
            return shape.getPoint(0);
        case 2:
            return (shape.getPoint(0) + shape.getPoint(1)) / 2.f;
        default: // more than two points
            sf::Vector2f centroid;
            float    twiceArea = 0;

            auto previousPoint = shape.getPoint(count - 1);
            for (std::size_t i = 0; i < count; ++i)
            {
                const auto  currentPoint = shape.getPoint(i);
                const float product      = cross(previousPoint, currentPoint);
                twiceArea += product;
                centroid += (currentPoint + previousPoint) * product;

                previousPoint = currentPoint;
            }

            if (twiceArea != 0.f) {
                return centroid / 3.f / twiceArea;
            }

            // Fallback for no area - find the center of the bounding box
            auto minPoint = shape.getPoint(0);
            auto maxPoint = minPoint;
            for (std::size_t i = 1; i < count; ++i) {
                const auto currentPoint = shape.getPoint(i);
                minPoint.x              = std::min(minPoint.x, currentPoint.x);
                maxPoint.x              = std::max(maxPoint.x, currentPoint.x);
                minPoint.y              = std::min(minPoint.y, currentPoint.y);
                maxPoint.y              = std::max(maxPoint.y, currentPoint.y);
            }
            return (maxPoint + minPoint) / 2.f;
    }
}

template<class T>
struct Line {
    sf::Vector2<T> A;
    sf::Vector2<T> B;
};

template<class T>
constexpr bool intersects(const Line<T>& ln1, const Line<T>& ln2) {
    // Calculate the cross product of the vectors formed by each line.
    const sf::Vector2<T> P2BA = ln2.B - ln2.A;
    const T crossProduct1 = cross(P2BA, ln1.A - ln2.A);
    const T crossProduct2 = cross(P2BA, ln1.B - ln2.A);
    constexpr T Zero{};

    // Check if the signs of the cross products are different or
    // if any of the endpoints of one line lie on the other line.
    return ((crossProduct1 > Zero && crossProduct2 < Zero) || (crossProduct1 < Zero && crossProduct2 > Zero))
        || (crossProduct1 == Zero && std::min(ln2.A.x, ln2.B.x) <= ln1.A.x && ln1.A.x <= std::max(ln2.A.x, ln2.B.x) && std::min(ln2.A.y, ln2.B.y) <= ln1.A.y && ln1.A.y <= std::max(ln2.A.y, ln2.B.y))
        || (crossProduct2 == Zero && std::min(ln2.A.x, ln2.B.x) <= ln1.B.x && ln1.B.x <= std::max(ln2.A.x, ln2.B.x) && std::min(ln2.A.y, ln2.B.y) <= ln1.B.y && ln1.B.y <= std::max(ln2.A.y, ln2.B.y));
}

template<class T>
constexpr std::optional<sf::Vector2<T>> intersection_point(const Line<T>& ln1, const Line<T>& ln2) {
    // Calculate the cross product of the vectors formed by each line.
    const sf::Vector2<T> P2BA = ln2.B - ln2.A;
    const T crossProduct1 = cross(P2BA, ln1.A - ln2.A);
    const T crossProduct2 = cross(P2BA, ln1.B - ln2.A);
    constexpr T Zero{};

    // Check if the signs of the cross products are different
    if (!((crossProduct1 > Zero && crossProduct2 < Zero) || (crossProduct1 < Zero && crossProduct2 > Zero))) return std::nullopt;

    // Calculate the intersection point.
    float denom = (ln1.A.x - ln1.B.x) * (ln2.A.y - ln2.B.y) - (ln1.A.y - ln1.B.y) * (ln2.A.x - ln2.B.x);

    if (denom == T{}) {
        // parallel, no intersection point.
        return std::nullopt;
    }

    float parameterized_distance = ((ln1.A.x - ln2.A.x) * (ln2.A.y - ln2.B.y) - (ln1.A.y - ln2.A.y) * (ln2.A.x - ln2.B.x)) / denom;
    float x = ln1.A.x + parameterized_distance * (ln1.B.x - ln1.A.x);
    float y = ln1.A.y + parameterized_distance * (ln1.B.y - ln1.A.y);
    return sf::Vector2f{x, y};
}
