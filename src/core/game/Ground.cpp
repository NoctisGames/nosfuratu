//
//  Ground.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/5/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "Ground.h"
#include "EntityAnchor.h"
#include "EntityUtils.h"

Ground* Ground::create(int gridX, int gridY, int type)
{
    switch ((GroundType)type)
    {
        case GroundType_CaveExtraDeepEndLeft:
            return new CaveExtraDeepEndLeft(gridX);
        case GroundType_CaveExtraDeepSmall:
            return new CaveExtraDeepSmall(gridX);
        case GroundType_CaveExtraDeepMedium:
            return new CaveExtraDeepMedium(gridX);
        case GroundType_CaveExtraDeepLarge:
            return new CaveExtraDeepLarge(gridX);
        case GroundType_CaveExtraDeepEndRight:
            return new CaveExtraDeepEndRight(gridX);
            
        case GroundType_CaveDeepEndLeft:
            return new CaveDeepEndLeft(gridX);
        case GroundType_CaveDeepSmall:
            return new CaveDeepSmall(gridX);
        case GroundType_CaveDeepMedium:
            return new CaveDeepMedium(gridX);
        case GroundType_CaveDeepLarge:
            return new CaveDeepLarge(gridX);
        case GroundType_CaveDeepEndRight:
            return new CaveDeepEndRight(gridX);
            
        case GroundType_CaveEndLeft:
            return new CaveEndLeft(gridX);
        case GroundType_CaveSmall:
            return new CaveSmall(gridX);
        case GroundType_CaveMedium:
            return new CaveMedium(gridX);
        case GroundType_CaveLarge:
            return new CaveLarge(gridX);
        case GroundType_CaveEndRight:
            return new CaveEndRight(gridX);
            
        case GroundType_CaveRaisedEndLeft:
            return new CaveRaisedEndLeft(gridX);
        case GroundType_CaveRaisedSmall:
            return new CaveRaisedSmall(gridX);
        case GroundType_CaveRaisedMedium:
            return new CaveRaisedMedium(gridX);
        case GroundType_CaveRaisedLarge:
            return new CaveRaisedLarge(gridX);
        case GroundType_CaveRaisedEndRight:
            return new CaveRaisedEndRight(gridX);
            
        case GroundType_GrassWithCaveEndLeft:
            return new GrassWithCaveEndLeft(gridX);
        case GroundType_GrassWithCaveSmall:
            return new GrassWithCaveSmall(gridX);
        case GroundType_GrassWithCaveMedium:
            return new GrassWithCaveMedium(gridX);
        case GroundType_GrassWithCaveLarge:
            return new GrassWithCaveLarge(gridX);
        case GroundType_GrassWithCaveEndRight:
            return new GrassWithCaveEndRight(gridX);
            
        case GroundType_GrassWithoutCaveEndLeft:
            return new GrassWithoutCaveEndLeft(gridX);
        case GroundType_GrassWithoutCaveSmall:
            return new GrassWithoutCaveSmall(gridX);
        case GroundType_GrassWithoutCaveMedium:
            return new GrassWithoutCaveMedium(gridX);
        case GroundType_GrassWithoutCaveLarge:
            return new GrassWithoutCaveLarge(gridX);
        case GroundType_GrassWithoutCaveEndRight:
            return new GrassWithoutCaveEndRight(gridX);
    }
    
    assert(false);
}

Ground::Ground(int gridX, int gridY, int gridWidth, int gridHeight, float boundsHeightFactor, GroundType type, GroundSoundType groundSoundType) : GridLockedPhysicalEntity(gridX, gridY, gridWidth, gridHeight), m_fBoundsHeightFactor(boundsHeightFactor), m_type(type), m_groundSoundType(groundSoundType)
{
    updateBounds();
}

void Ground::updateBounds()
{
    m_bounds->setHeight(getHeight());
    
    PhysicalEntity::updateBounds();
    
    m_bounds->setHeight(getHeight() * m_fBoundsHeightFactor);
}

GroundType Ground::getType()
{
    return m_type;
}

GroundSoundType Ground::getGroundSoundType()
{
    return m_groundSoundType;
}