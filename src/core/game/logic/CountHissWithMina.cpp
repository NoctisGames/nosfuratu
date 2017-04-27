//
//  CountHissWithMina.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 10/26/16.
//  Copyright © 2017 Noctis Games. All rights reserved.
//

#include "pch.h"

#include "CountHissWithMina.h"

CountHissWithMina* CountHissWithMina::create(int gridX, int gridY, int type)
{
    return new CountHissWithMina(gridX, gridY, 32, 32, 0, 0.1953125f, 1, 0.8046875f);
}

CountHissWithMina::CountHissWithMina(int gridX, int gridY, int gridWidth, int gridHeight, float boundsX, float boundsY, float boundsWidth, float boundsHeight) : GridLockedPhysicalEntity(gridX, gridY, gridWidth, gridHeight, boundsX, boundsY, boundsWidth, boundsHeight),
m_type(-1),
m_isMoving(false),
m_isFacingLeft(false)
{
    // Empty
}

void CountHissWithMina::update(float deltaTime)
{
    GridLockedPhysicalEntity::update(deltaTime);
    
    updateBounds();
}

bool CountHissWithMina::isMoving()
{
    return m_isMoving;
}

void CountHissWithMina::beginMovement()
{
    m_isMoving = true;
    
    m_velocity.setX(m_isMoving ? 4.0f : 0);
}

bool CountHissWithMina::isFacingLeft()
{
    return m_isFacingLeft;
}

void CountHissWithMina::faceLeft()
{
    m_isFacingLeft = true;
}

int CountHissWithMina::getType()
{
    return m_type;
}

RTTI_IMPL(CountHissWithMina, GridLockedPhysicalEntity);
