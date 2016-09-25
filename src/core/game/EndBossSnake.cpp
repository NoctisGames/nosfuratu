//
//  EndBossSnake.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/24/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "EndBossSnake.h"
#include "Game.h"
#include "OverlapTester.h"
#include "Assets.h"
#include "Jon.h"
#include "macros.h"

#include <math.h>

EndBossSnake::EndBossSnake(int gridX, int gridY, int gridWidth, int gridHeight) : GridLockedPhysicalEntity(gridX, gridY, gridWidth, gridHeight),
m_game(nullptr),
m_state(EndBossSnakeState_Sleeping),
m_color(1, 1, 1, 1),
m_iDamage(0),
m_type(-1)
{
    // Empty
}

void EndBossSnake::update(float deltaTime)
{
    GridLockedPhysicalEntity::update(deltaTime);
}

void EndBossSnake::setState(EndBossSnakeState state)
{
    m_state = state;
    m_fStateTime = 0.0f;
}
