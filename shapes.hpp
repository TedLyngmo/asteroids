#pragma once

#include "helpers.hpp"

#include <SFML/Graphics.hpp>

#include <cstddef>
#include <initializer_list>
#include <utility>
#include <vector>

class Polygon : public sf::ConvexShape {
public:

    inline Polygon(std::initializer_list<sf::Vector2f> pnts) : sf::ConvexShape(pnts.size()) {
        for(unsigned idx = 0; idx < pnts.size(); ++idx) {
            setPoint(idx, pnts.begin()[idx]);
        }
        setOrigin(findCentroid(pnts.begin(), pnts.end()));
    }

    bool isInside(sf::Vector2f point) const {
        for(std::size_t i = 0, j = getPointCount() - 1; i < getPointCount(); j = i++) {
            auto first = getPoint(i);
            auto second = getPoint(j);
            // assuming all points are stored in clockwise order a point "left" of a line in the polygon is outside the polygon
            if((second.x - first.x) * (point.y - first.y) > (second.y - first.y) * (point.x - first.x)) return false;

        }
        return true;
    }

};
