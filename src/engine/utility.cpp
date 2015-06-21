
#include "utility.hpp"

#ifdef Rational_FOUND
  #include <Rational.hpp>
#endif

#ifndef NDEBUG
  #include <iostream>
#endif

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

bool Utility::lineIntersect(const sf::Vector2f& point0, const sf::Vector2f& point1, const sf::Vector2f& point2, const sf::Vector2f& point3)
{
    return lineIntersect(point0.x, point0.y, point1.x, point1.y, point2.x, point2.y, point3.x, point3.y);
}

bool Utility::lineIntersect(float x_0, float y_0, float x_1, float y_1, float x_2, float y_2, float x_3, float y_3)
{
    // get slope of both lines
    float slope0 = (y_1 - y_0) / (x_1 - x_0);
    float slope1 = (y_3 - y_2) / (x_3 - x_2);

    // get the intersept of both lines
    float inter0 = y_0 - slope0 * x_0;
    float inter1 = y_2 - slope1 * x_2;

    // check if line zero is on top of line one
    float interDiff = inter0 - inter1;
    float slopeDiff = slope1 - slope0;

    if((interDiff > -0.001 && interDiff < 0.001 &&
       slopeDiff > -0.001 && slopeDiff < 0.001) ||
       (std::isinf(slope0) && std::isinf(slope1) &&
       x_0 - x_2 > -0.001 && x_0 - x_2 < 0.001))
    {
#ifndef NDEBUG
        std::cout << "Line is on top of line.\n";
#endif
        return true;
    }
    // check if line never intersects the other
    else if((slopeDiff > -0.001 && slopeDiff < 0.001) ||
            (std::isinf(slope0) && std::isinf(slope1) &&
            (x_0 - x_2 > 0.001 || x_0 - x_2 < -0.001)))
    {
#ifndef NDEBUG
        std::cout << "Line never intersects line.\n";
#endif
        return false;
    }

    // find intersection
    float intersect_x;
    float intersect_y;
    if(std::isinf(slope0))
    {
        intersect_x = x_0;
        intersect_y = slope1 * intersect_x + inter1;
    }
    else if(std::isinf(slope1))
    {
        intersect_x = x_2;
        intersect_y = slope0 * intersect_x + inter0;
    }
    else
    {
        intersect_x = interDiff / slopeDiff;
        intersect_y = slope0 * intersect_x + inter0;
    }

#ifndef NDEBUG
    std::cout << intersect_x << ' ' << intersect_y << '\n';
#endif

    // check if intersection is within bounds
    const float& maxX_0 = x_0 > x_1 ? x_0 : x_1;
    const float& minX_0 = x_1 == maxX_0 ? x_0 : x_1;
    const float& maxX_1 = x_2 > x_3 ? x_2 : x_3;
    const float& minX_1 = x_3 == maxX_1 ? x_2 : x_3;

    const float& maxY_0 = y_0 > y_1 ? y_0 : y_1;
    const float& minY_0 = y_1 == maxY_0 ? y_0 : y_1;
    const float& maxY_1 = y_2 > y_3 ? y_2 : y_3;
    const float& minY_1 = y_3 == maxY_1 ? y_2 : y_3;

#ifndef NDEBUG
    std::cout << "Max X " << maxX_0 << ' ' << maxX_1 << '\n';
    std::cout << "Min X " << minX_0 << ' ' << minX_1 << '\n';
    std::cout << "Max Y " << maxY_0 << ' ' << maxY_1 << '\n';
    std::cout << "Min Y " << minY_0 << ' ' << minY_1 << '\n';
#endif

    return (minX_0 <= intersect_x && maxX_0 >= intersect_x &&
            minX_1 <= intersect_x && maxX_1 >= intersect_x &&
            minY_0 <= intersect_y && maxY_0 >= intersect_y &&
            minY_1 <= intersect_y && maxY_1 >= intersect_y);
}


bool Utility::lineIntersect(const sf::Vector2i& point0, const sf::Vector2i& point1, const sf::Vector2i& point2, const sf::Vector2i& point3)
{
    return lineIntersect(point0.x, point0.y, point1.x, point1.y, point2.x, point2.y, point3.x, point3.y);
}

bool Utility::lineIntersect(int x_0, int y_0, int x_1, int y_1, int x_2, int y_2, int x_3, int y_3)
{
#ifdef Rational_FOUND
    // get slope
    Rational slope0(y_1 - y_0, x_1 - x_0);
    Rational slope1(y_3 - y_2, x_3 - x_2);

    // get intersept
    Rational inter0(slope0 * -x_0 + y_0);
    Rational inter1(slope1 * -x_2 + y_2);

    // check if line is on top of other
    Rational interDiff(inter0 - inter1);
    Rational slopeDiff(slope1 - slope0);
    if((interDiff.toFloat() == 0.0f && slopeDiff.toFloat() == 0.0f) ||
       (slope0.isInvalid() && slope1.isInvalid() &&
        x_0 - x_2 == 0))
    {
#ifndef NDEBUG
        std::cout << "Line is on top of line.\n";
#endif
        return true;
    }
    // check if line never intersects other
    else if((slopeDiff.toFloat() == 0.0f) ||
            (slope0.isInvalid() && slope1.isInvalid() &&
            x_0 - x_2 != 0))
    {
#ifndef NDEBUG
        std::cout << "Line never intersects line.\n";
#endif
        return false;
    }

    // find intersection
    Rational intersect_x;
    Rational intersect_y;

    if(slope0.isInvalid())
    {
        intersect_x = Rational(x_0);
        intersect_y = slope1 * intersect_x + inter1;
    }
    else if(slope1.isInvalid())
    {
        intersect_x = Rational(x_2);
        intersect_y = slope0 * intersect_x + inter0;
    }
    else
    {
        intersect_x = interDiff / slopeDiff;
        intersect_y = slope0 * intersect_x + inter0;
    }

#ifndef NDEBUG
    std::cout << intersect_x.toString() << ' ' << intersect_y.toString() << '\n';
#endif

    // check if intersection is within bounds
    const int& maxX_0 = x_0 > x_1 ? x_0 : x_1;
    const int& minX_0 = x_1 == maxX_0 ? x_0 : x_1;
    const int& maxX_1 = x_2 > x_3 ? x_2 : x_3;
    const int& minX_1 = x_3 == maxX_1 ? x_2 : x_3;

    const int& maxY_0 = y_0 > y_1 ? y_0 : y_1;
    const int& minY_0 = y_1 == maxY_0 ? y_0 : y_1;
    const int& maxY_1 = y_2 > y_3 ? y_2 : y_3;
    const int& minY_1 = y_3 == maxY_1 ? y_2 : y_3;

#ifndef NDEBUG
    std::cout << "Max X " << maxX_0 << ' ' << maxX_1 << '\n';
    std::cout << "Min X " << minX_0 << ' ' << minX_1 << '\n';
    std::cout << "Max Y " << maxY_0 << ' ' << maxY_1 << '\n';
    std::cout << "Min Y " << minY_0 << ' ' << minY_1 << '\n';
#endif

    return (intersect_x >= minX_0 && intersect_x <= maxX_0 &&
            intersect_x >= minX_1 && intersect_x <= maxX_1 &&
            intersect_y >= minY_0 && intersect_y <= maxY_0 &&
            intersect_y >= minY_1 && intersect_y <= maxY_1);
#else
    return lineIntersect((float) x_0, (float) y_0, (float) x_1, (float) y_1, (float) x_2, (float) y_2, (float) x_3, (float) y_3);
#endif
}

bool Utility::lineCircleIntersect(const sf::Vector2f& point0, const sf::Vector2f& point1, const sf::Vector2f& center, float radius)
{
    return lineCircleIntersect(point0.x, point0.y, point1.x, point1.y, center.x, center.y, radius);
}

bool Utility::lineCircleIntersect(float x_0, float y_0, float x_1, float y_1, float x_2, float y_2, float radius)
{
    // get line 0
    float slope_0 = (y_1 - y_0) / (x_1 - x_0);
    float inter_0 = y_0 - x_0 * slope_0;

    // get tangent line (line 1)
    float slope_1;
    if(std::isinf(slope_0))
    {
        slope_1 = 0.0f;
    }
    else if(slope_0 > -0.001f && slope_0 < 0.001f)
    {
        slope_1 = INFINITY;
    }
    else
    {
        slope_1 = -1.0f / slope_0;
    }
    float inter_1 = y_2 - x_2 * slope_1;

    // get intersection
    float intersect_x;
    float intersect_y;
    if(slope_1 > -0.001f && slope_1 < 0.001f)
    {
        intersect_x = x_0;
        intersect_y = y_2;
    }
    else if(slope_0 > -0.001f && slope_0 < 0.001f)
    {
        intersect_x = x_2;
        intersect_y = y_0;
    }
    else
    {
        intersect_x = (inter_0 - inter_1) / (slope_1 - slope_0);
        intersect_y = intersect_x * slope_1 + inter_1;
    }

    // check if intersection is within bounds
    float& maxX = x_0 > x_1 ? x_0 : x_1;
    float& minX = x_1 == maxX ? x_0 : x_1;

    float& maxY = y_0 > y_1 ? y_0 : y_1;
    float& minY = y_1 == maxY ? y_0 : y_1;

    if(minX <= intersect_x && maxX >= intersect_x &&
       minY <= intersect_y && maxY >= intersect_y)
    {
        // within bounds
        // need to check if magnitude of intersect to center is <= radius
        float vector_x = intersect_x - x_2;
        float vector_y = intersect_y - y_2;
        if(std::sqrt(vector_x * vector_x + vector_y * vector_y) <= radius)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        // intersection not on line segment
        // check if magnitude of both ends of segment to center is<= radius
        float vector_x = x_0 - x_2;
        float vector_y = y_0 - y_2;
        if(std::sqrt(vector_x * vector_x + vector_y * vector_y) <= radius)
        {
            return true;
        }
        vector_x = x_1 - x_2;
        vector_y = y_1 - y_2;
        if(std::sqrt(vector_x * vector_x + vector_y * vector_y) <= radius)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool Utility::isWithinPolygon(const sf::VertexArray& coords, float x_0, float y_0)
{
    unsigned int j;
    float v0_x, v0_y;
    float v1_x, v1_y;
    bool resultPositive;
    for(unsigned int i = 0; i < coords.getVertexCount(); ++i)
    {
        j = (i + 1) % coords.getVertexCount();

        v0_x = coords[j].position.x - coords[i].position.x;
        v0_y = coords[j].position.y - coords[i].position.y;

        v1_x = x_0 - coords[i].position.x;
        v1_y = y_0 - coords[j].position.y;

        if(i == 0)
        {
            resultPositive = v0_x * v1_y - v1_x * v0_y > 0.0f;
        }
        else if(resultPositive != (v0_x * v1_y - v1_x * v0_y > 0.0f))
        {
            return false;
        }
    }

    return true;
}

