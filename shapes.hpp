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
        for(std::size_t idx = 0; idx < pnts.size(); ++idx) {
            setPoint(idx, pnts.begin()[idx]);
        }
        setOrigin(findCentroid(pnts.begin(), pnts.end()));
    }

    bool isInside(sf::Vector2f point) const {
        const auto& trans = getTransform();

        std::vector<sf::Vector2f> tpnts;
        tpnts.reserve(getPointCount());

        for(std::size_t idx = 0, cnt = getPointCount(); idx < cnt; ++idx) {
            tpnts.push_back(trans.transformPoint(getPoint(idx)));
        }

        for(std::size_t i = 0, j = tpnts.size() - 1; i < tpnts.size(); j = i++) {
            auto first = tpnts[i];
            auto second = tpnts[j];
            // assuming all points are stored in clockwise order a point "left" of a line in the polygon is outside the polygon
            if((second.x - first.x) * (point.y - first.y) > (second.y - first.y) * (point.x - first.x)) return false;

        }
        return true;
    }

};
