#pragma once

#include "helpers.hpp"

#include <SFML/Graphics.hpp>

#include "helpers.hpp"
#include "pi.hpp"

#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <utility>
#include <vector>
#include <ostream>

#include <iostream>

class Polygon : public sf::ConvexShape {
public:

    template<class It>
    Polygon(It first, It last) : sf::ConvexShape(std::distance(first, last)), tpnts(std::distance(first, last)) {
        std::size_t idx = 0;
        for(auto curr = first; curr != last; ++curr, ++idx) {
            setPoint(idx, *curr);
        }
        setOrigin(getCentroid(*this));
        // circumference
        float C = 0.f;
        for(auto prev = std::prev(last), curr = first; curr != last; prev = curr++) {
            C += length(*curr - *prev);
        }
        // radius
        auto r = C / twopi;
        // volume
        volume = r * r * r * pi * 4.f / 3.f;
    }

    inline Polygon(std::initializer_list<sf::Vector2f> pnts) : Polygon(pnts.begin(), pnts.end()) {}

    // Copyright 2000 softSurfer, 2012 Dan Sunday
    // This code may be freely used and modified for any purpose
    // providing that this copyright notice is included with it.
    // SoftSurfer makes no warranty for this code, and cannot be held
    // liable for any real or imagined damage resulting from its use.
    // Users of this code must verify correctness for their application.
    // https://web.archive.org/web/20130126163405/http://geomalgorithms.com/a03-_inclusion.html
    //
    //    Return: >0 for point  left of the line through P0 and P1
    //            =0 for point  on the line
    //            <0 for point  right of the line
    static inline float sideOfLine(sf::Vector2f point, sf::Vector2f P0, sf::Vector2f P1) {
        return (P1.x - P0.x) * (point.y - P0.y) - (point.x -  P0.x) * (P1.y - P0.y);
    }

    // wn_PnPoly(): winding number test for a point in a polygon
    //      Input:   P = a point,
    //      Return:  wn = the winding number (=0 only when P is outside)
    inline int wn_PnPoly(sf::Vector2f P) const
    {
        int    wn = 0;    // the  winding number counter

        // loop through all edges of the polygon
        auto prev = tpnts.back();
        for(auto& curr : tpnts) {
            if (curr.y <= P.y) {                           // start y <= P.y
                if (prev.y  > P.y)                         // an upward crossing
                     if (sideOfLine(P, curr, prev) > 0.f)  // P left of  edge
                         ++wn;                             // have  a valid up intersect
            }
            else {                                         // start y > P.y (no test needed)
                if (prev.y  <= P.y)                        // a downward crossing
                     if (sideOfLine(P, curr, prev) < 0.f)  // P right of  edge
                         --wn;                             // have  a valid down intersect
            }
            prev = curr;
        }
        return wn;
    }

    inline bool isInside(sf::Vector2f point) const {
        sf::FloatRect rect = getGlobalBounds();
        if(not rect.contains(point)) return false;
        Transform();
        return wn_PnPoly(point) != 0;
    }

    inline float getVolume() const { return volume; }

public:
    template<class Shapeish>
    bool intersects(const Shapeish& s) const {
        if(not getGlobalBounds().intersects(s.getGlobalBounds())) return false;

        auto count = s.getPointCount();
        if(count < 2) return false;
        Transform();
        auto prev = s.getPoint(count - 1);
        for(std::size_t i = 0; i < count; ++i) {
            if(wn_PnPoly(prev)) return true;
            auto curr = s.getPoint(i);
            if(intersects_line(Line<float>{prev, curr})) return true;
            prev = curr;
        }
        return false;
    }

    inline friend std::ostream& operator<<(std::ostream& os, const Polygon& p) {
        auto pos = p.getPosition();
        return os << "Polygon{" << pos.x << ',' << pos.y << '}';
    }

private:
    float volume;

    template<class T>
    constexpr bool intersects_line(const Line<T>& line) const {
        auto prev = tpnts.back();
        for(auto& curr : tpnts) {
            if(line_intersects(line, Line<float>{prev, curr})) return true;
            prev = curr;
        }
        return false;
    }

    inline void Transform() const {
        const auto& trans = getTransform();
        if(m_trans == trans) return;
        m_trans = trans;

        for(std::size_t idx = 0, cnt = getPointCount(); idx < cnt; ++idx) {
            tpnts[idx] = m_trans.transformPoint(getPoint(idx));
        }
    }

    mutable std::vector<sf::Vector2f> tpnts;
    mutable sf::Transform m_trans;
};
