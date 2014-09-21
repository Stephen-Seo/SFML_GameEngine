
#include <cassert>
#include <iostream>
#include <typeindex>
#include <memory>

#include "gtest/gtest.h"

#include "ec/entity.hpp"
#include "ec/engine.hpp"
#include "ec/cPosition.hpp"
#include "ec/cVelocity.hpp"
#include "ec/cAcceleration.hpp"
#include "ec/nMove.hpp"
#include "context.hpp"
#include "resourceIdentifiers.hpp"
#include "resourceManager.hpp"
#include "musicPlayer.hpp"
#include "soundPlayer.hpp"

TEST(ECTest, ECUpdate)
{
    Engine engine;
    engine.addSystem(std::unique_ptr<Node>(new nMove));

    Entity* entity = new Entity;

    cPosition* pos = new cPosition;
    cVelocity* vel = new cVelocity;
    cAcceleration* acc = new cAcceleration;

    pos->x = 0;
    pos->y = 0;
    pos->rot = 0;

    vel->x = 0;
    vel->y = 0;
    vel->rot = 0;

    acc->x = 1.0f;
    acc->y = 1.0f;
    acc->rot = 1.0f;

    entity->addComponent(std::type_index(typeid(cPosition)), std::unique_ptr<Component>(pos));
    entity->addComponent(std::type_index(typeid(cVelocity)), std::unique_ptr<Component>(vel));
    entity->addComponent(std::type_index(typeid(cAcceleration)), std::unique_ptr<Component>(acc));

    engine.addEntity(std::unique_ptr<Entity>(entity));

    sf::RenderWindow window;
    ResourceManager rManager(nullptr, GameResources::DEFAULT);
    MusicPlayer mPlayer;
    SoundPlayer sPlayer;
    bool isQuitting;

    engine.update(sf::seconds(1.0f), Context(window, rManager, mPlayer, sPlayer, engine, isQuitting));

    EXPECT_NE(0.0f, pos->x);
    EXPECT_EQ(1.0f, pos->x);

    engine.update(sf::seconds(1.0f), Context(window, rManager, mPlayer, sPlayer, engine, isQuitting));

    EXPECT_EQ(2.0f, vel->y);
    EXPECT_EQ(3.0f, pos->y);
    EXPECT_EQ(3.0f, pos->rot);
}
