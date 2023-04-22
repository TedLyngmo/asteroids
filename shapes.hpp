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
        setOrigin(findCentroid(pnts.begin(), pnts.size()));
    }

};
