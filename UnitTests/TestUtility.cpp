
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

TEST(UtilityTest, lineIntersect)
{
    float x_0 = 0.0f;
    float y_0 = 0.0f;

    float x_1 = 1.0f;
    float y_1 = 1.0f;

    float x_2 = 0.0f;
    float y_2 = 1.0f;

    float x_3 = 1.0f;
    float y_3 = 0.0f;

    ASSERT_TRUE(Utility::lineIntersect(x_0, y_0, x_1, y_1, x_2, y_2, x_3, y_3));

    x_2 = 1.1f;
    y_2 = 0.9f;

    x_3 = 0.0f;
    y_3 = 0.9f;

    ASSERT_TRUE(Utility::lineIntersect(x_0, y_0, x_1, y_1, x_2, y_2, x_3, y_3));

    x_1 = 0.0f;
    y_1 = 1.0f;

    ASSERT_TRUE(Utility::lineIntersect(x_0, y_0, x_1, y_1, x_2, y_2, x_3, y_3));

    x_2 = 0.0f;
    y_2 = 1.0f;

    x_3 = 0.0f;
    y_3 = 0.0f;

    ASSERT_TRUE(Utility::lineIntersect(x_0, y_0, x_1, y_1, x_2, y_2, x_3, y_3));

    x_2 = 1.0f;
    y_2 = 1.0f;

    x_3 = 1.0f;
    y_3 = 0.0f;

    ASSERT_FALSE(Utility::lineIntersect(x_0, y_0, x_1, y_1, x_2, y_2, x_3, y_3));

    x_1 = 1.0f;
    y_1 = 0.0f;

    x_3 = 0.0f;
    y_3 = 1.0f;

    ASSERT_FALSE(Utility::lineIntersect(x_0, y_0, x_1, y_1, x_2, y_2, x_3, y_3));

    x_1 = 1.0f;
    y_1 = 1.0f;

    x_2 = 1.0f;
    y_2 = 0.0f;

    x_3 = 2.0f;
    y_3 = 1.0f;

    ASSERT_FALSE(Utility::lineIntersect(x_0, y_0, x_1, y_1, x_2, y_2, x_3, y_3));

    x_1 = 1.0f;
    y_1 = 0.0f;

    x_2 = 1.0f;
    y_2 = 0.0f;

    x_3 = 0.0f;
    y_3 = 0.0f;

    ASSERT_TRUE(Utility::lineIntersect(x_0, y_0, x_1, y_1, x_2, y_2, x_3, y_3));

    x_1 = 2.0f;
    y_1 = 1.5f;

    x_2 = 2.0f;
    y_2 = 1.5f;

    ASSERT_TRUE(Utility::lineIntersect(x_0, y_0, x_1, y_1, x_2, y_2, x_3, y_3));

    x_1 = 2.0f;
    y_1 = 2.0f;

    x_2 = 3.0f;
    y_2 = 0.0f;

    x_3 = 1.1f;
    y_3 = 0.9f;

    ASSERT_FALSE(Utility::lineIntersect(x_0, y_0, x_1, y_1, x_2, y_2, x_3, y_3));
}

TEST(UtilityTest, lineIntersectRational)
{
    int x_0 = 0;
    int y_0 = 0;

    int x_1 = 1;
    int y_1 = 0;

    int x_2 = 1;
    int y_2 = 1;

    int x_3 = 1;
    int y_3 = 0;

    ASSERT_TRUE(Utility::lineIntersect(x_0, y_0, x_1, y_1, x_2, y_2, x_3, y_3));

    x_2 = 1;
    y_2 = 0;

    x_3 = 0;
    y_3 = 0;

    ASSERT_TRUE(Utility::lineIntersect(x_0, y_0, x_1, y_1, x_2, y_2, x_3, y_3));

    y_2 = 1;

    y_3 = 1;

    ASSERT_FALSE(Utility::lineIntersect(x_0, y_0, x_1, y_1, x_2, y_2, x_3, y_3));

    x_1 = 0;
    y_1 = 1;

    ASSERT_TRUE(Utility::lineIntersect(x_0, y_0, x_1, y_1, x_2, y_2, x_3, y_3));

    x_2 = 0;
    y_2 = 1;

    x_3 = 0;
    y_3 = 1;

    ASSERT_TRUE(Utility::lineIntersect(x_0, y_0, x_1, y_1, x_2, y_2, x_3, y_3));

    x_2 = 1;

    x_3 = 1;

    ASSERT_FALSE(Utility::lineIntersect(x_0, y_0, x_1, y_1, x_2, y_2, x_3, y_3));

    x_1 = 1;
    y_1 = 1;

    x_2 = 1;
    y_2 = 0;

    x_3 = 0;
    y_3 = 1;

    ASSERT_TRUE(Utility::lineIntersect(x_0, y_0, x_1, y_1, x_2, y_2, x_3, y_3));

    y_2 = 1;

    y_3 = 0;

    ASSERT_TRUE(Utility::lineIntersect(x_0, y_0, x_1, y_1, x_2, y_2, x_3, y_3));

    x_2 = 2;
    y_2 = 1;

    x_3 = 1;
    y_3 = 0;

    ASSERT_FALSE(Utility::lineIntersect(x_0, y_0, x_1, y_1, x_2, y_2, x_3, y_3));
}

