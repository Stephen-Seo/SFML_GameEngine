
#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <SFML/Graphics.hpp>

namespace Utility
{
    // Coords must be an array of sf::Vector2f of size 4, all in clockwise/counterclockwise direction
    bool isWithinTransformedRectangle(const sf::Vector2f* coords, sf::Vector2f point);
}

#endif
