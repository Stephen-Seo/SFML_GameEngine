
#include "gtest/gtest.h"

#include <engine/utility.hpp>

TEST(UtilityTest, WithinTransformedRectangle)
{
    sf::Vector2f coords[4];

    coords[0].x = 0.0f;
    coords[0].y = 50.0f;

    coords[1].x = 50.0f;
    coords[1].y = 100.0f;

    coords[2].x = 100.0f;
    coords[2].y = 50.0f;

    coords[3].x = 50.0f;
    coords[3].y = 0.0f;

    EXPECT_TRUE(Utility::isWithinTransformedRectangle(coords, sf::Vector2f(50.0f, 50.0f)));

    EXPECT_FALSE(Utility::isWithinTransformedRectangle(coords, sf::Vector2f(0.0f, 100.0f)));
    EXPECT_FALSE(Utility::isWithinTransformedRectangle(coords, sf::Vector2f(100.0f, 100.0f)));
    EXPECT_FALSE(Utility::isWithinTransformedRectangle(coords, sf::Vector2f(100.0f, 0.0f)));
    EXPECT_FALSE(Utility::isWithinTransformedRectangle(coords, sf::Vector2f(0.0f, 0.0f)));

    coords[0].y = 0.0f;

    coords[1].x = 0.0f;
    coords[1].y = 2.0f;

    coords[2].x = 2.0f;
    coords[2].y = 2.0f;

    coords[3].x = 2.0f;
    coords[3].y = 0.0f;

    EXPECT_TRUE(Utility::isWithinTransformedRectangle(coords, sf::Vector2f(1.0f, 1.0f)));

    EXPECT_FALSE(Utility::isWithinTransformedRectangle(coords, sf::Vector2f(-1.0f, 1.0f)));
    EXPECT_FALSE(Utility::isWithinTransformedRectangle(coords, sf::Vector2f(1.0f, 3.0f)));
    EXPECT_FALSE(Utility::isWithinTransformedRectangle(coords, sf::Vector2f(3.0f, 1.0f)));
    EXPECT_FALSE(Utility::isWithinTransformedRectangle(coords, sf::Vector2f(1.0f, -1.0f)));

    EXPECT_FALSE(Utility::isWithinTransformedRectangle(coords, sf::Vector2f(-1.0f, -1.0f)));
    EXPECT_FALSE(Utility::isWithinTransformedRectangle(coords, sf::Vector2f(-1.0f, 3.0f)));
    EXPECT_FALSE(Utility::isWithinTransformedRectangle(coords, sf::Vector2f(3.0f, 3.0f)));
    EXPECT_FALSE(Utility::isWithinTransformedRectangle(coords, sf::Vector2f(3.0f, -1.0f)));

    EXPECT_TRUE(Utility::isWithinTransformedRectangle(coords, sf::Vector2f(0.0f, 0.0f)));
    EXPECT_TRUE(Utility::isWithinTransformedRectangle(coords, sf::Vector2f(2.0f, 2.0f)));
}
