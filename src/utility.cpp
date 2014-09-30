
#include "utility.hpp"

bool Utility::isWithinTransformedRectangle(const sf::Vector2f* coords, sf::Vector2f point)
{
    int j;
    sf::Vector2f v1, v2;
    bool resultPositive;
    for(int i = 0; i < 4; ++i)
    {
        j = (i + 1) % 4;

        v1.x = coords[j].x - coords[i].x;
        v1.y = coords[j].y - coords[i].y;

        v2.x = point.x - coords[i].x;
        v2.y = point.y - coords[i].y;

        if(i == 0)
            resultPositive = v1.x * v2.y - v1.y * v2.x > 0.0f;
        else if(resultPositive != (v1.x * v2.y - v1.y * v2.x > 0.0f))
            return false;
    }

    return true;
}
