
#include <cassert>
#include <iostream>
#include <typeindex>
#include <memory>
#include <random>

#include "gtest/gtest.h"

#include <engine/ec/entity.hpp>
#include <engine/ec/engine.hpp>
#include <engine/ec/cPosition.hpp>
#include <engine/ec/cVelocity.hpp>
#include <engine/ec/cAcceleration.hpp>
#include <engine/ec/nMove.hpp>
#include <engine/context.hpp>
#include <engine/connection.hpp>
#include <engine/resourceIdentifiers.hpp>
#include <engine/resourceManager.hpp>
#include <engine/musicPlayer.hpp>
#include <engine/soundPlayer.hpp>

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

    ResourceManager rManager(nullptr, GameResources::DEFAULT);
    MusicPlayer mPlayer;
    SoundPlayer sPlayer;
    bool isQuitting;
    std::unique_ptr<Connection> connection;
    sf::Color clearColor;
    std::mt19937 randomEngine;

    engine.update(sf::seconds(1.0f), Context(nullptr, rManager, mPlayer, sPlayer, engine, isQuitting, connection, clearColor, randomEngine));

    EXPECT_NE(0.0f, pos->x);
    EXPECT_EQ(1.0f, pos->x);

    engine.update(sf::seconds(1.0f), Context(nullptr, rManager, mPlayer, sPlayer, engine, isQuitting, connection, clearColor, randomEngine));

    EXPECT_EQ(2.0f, vel->y);
    EXPECT_EQ(3.0f, pos->y);
    EXPECT_EQ(3.0f, pos->rot);
}
