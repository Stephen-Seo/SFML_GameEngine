
#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <SFML/Graphics.hpp>

namespace Utility
{
    // Coords must be an array of sf::Vector2f of size 4, all in clockwise/counterclockwise direction
    bool isWithinTransformedRectangle(const sf::Vector2f* coords, sf::Vector2f point);

    bool lineIntersect(const sf::Vector2f& point0, const sf::Vector2f& point1, const sf::Vector2f& point2, const sf::Vector2f& point3);
    bool lineIntersect(float x_0, float y_0, float x_1, float y_1, float x_2, float y_2, float x_3, float y_3);

    bool lineIntersect(const sf::Vector2i& point0, const sf::Vector2i& point1, const sf::Vector2i& point2, const sf::Vector2i& point3);
    bool lineIntersect(int x_0, int y_0, int x_1, int y_1, int x_2, int y_2, int x_3, int y_3);
}

#endif
