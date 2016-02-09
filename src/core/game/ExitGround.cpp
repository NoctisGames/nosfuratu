//
//  ExitGround.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/8/16.
//  Copyright (c) 2016 Gowen Game Dev. All rights reserved.
//

#include "ExitGround.h"
#include "EntityAnchor.h"
#include "EntityUtils.h"

ExitGround* ExitGround::create(int gridX, int gridY, int type)
{
    switch ((ExitGroundType)type)
    {
        case ExitGroundType_GrassWithCaveSmallExitMid:
            return new GrassWithCaveSmallExitMid(gridX);
        case ExitGroundType_GrassWithCaveSmallExitEnd:
            return new GrassWithCaveSmallExitEnd(gridX);
        case ExitGroundType_CaveSmallExit:
            return new CaveSmallExit(gridX);
            
        case ExitGroundType_CaveDeepSmallWaterfall:
            return new CaveDeepSmallWaterfall(gridX);
    }
    
    assert(false);
}

ExitGround::ExitGround(int gridX, int gridY, int gridWidth, int gridHeight, float boundsHeightFactor, ExitGroundType type, GroundSoundType groundSoundType) : GridLockedPhysicalEntity(gridX, gridY, gridWidth, gridHeight), m_fBoundsHeightFactor(boundsHeightFactor), m_type(type), m_groundSoundType(groundSoundType)
{
    updateBounds();
}

void ExitGround::updateBounds()
{
    m_bounds->setHeight(getHeight());
    
    PhysicalEntity::updateBounds();
    
    m_bounds->setHeight(getHeight() * m_fBoundsHeightFactor);
}

bool ExitGround::isJonLanding(Jon& jon, float deltaTime)
{
    float jonVelocityY = jon.getVelocity().getY();
    float jonBottomY = jon.getBounds().getBottom();
    float jonLeftX = jon.getBounds().getLeft();
    float jonRightX = jon.getBounds().getRight();
    float jonYDelta = fabsf(jonVelocityY * deltaTime);
    
    if (jonVelocityY <= 0)
    {
        if (OverlapTester::doRectanglesOverlap(jon.getBounds(), getBounds()))
        {
            float itemTop = getBounds().getTop();
            float padding = itemTop * .01f;
            padding += jonYDelta;
            float itemTopReq = itemTop - padding;
            float pitEntranceLeft = getBounds().getLeft() + getBounds().getWidth() * 0.2421875f;
            float pitEntranceRight = getBounds().getLeft() + getBounds().getWidth() * 0.75f;
            
            if (jonBottomY >= itemTopReq && (jonRightX < pitEntranceLeft || jonLeftX > pitEntranceRight))
            {
                jon.getPosition().setY(itemTop + jon.getBounds().getHeight() / 2 * .99f);
                jon.updateBounds();
                jon.setGroundSoundType(getGroundSoundType());
                
                return true;
            }
        }
    }
    
    return false;
}

ExitGroundType ExitGround::getType()
{
    return m_type;
}

GroundSoundType ExitGround::getGroundSoundType()
{
    return m_groundSoundType;
}