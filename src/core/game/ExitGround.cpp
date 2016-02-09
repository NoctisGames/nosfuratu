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

ExitGroundType ExitGround::getType()
{
    return m_type;
}

GroundSoundType ExitGround::getGroundSoundType()
{
    return m_groundSoundType;
}