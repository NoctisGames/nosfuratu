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
    Ground* pGround;
    
    switch ((GroundType)type)
    {
        case GroundType_CaveExtraDeepEndLeft:
            pGround = new CaveExtraDeepEndLeft(gridX);
            break;
        case GroundType_CaveExtraDeepSmall:
            pGround = new CaveExtraDeepSmall(gridX);
            break;
        case GroundType_CaveExtraDeepMedium:
            pGround = new CaveExtraDeepMedium(gridX);
            break;
        case GroundType_CaveExtraDeepLarge:
            pGround = new CaveExtraDeepLarge(gridX);
            break;
        case GroundType_CaveExtraDeepEndRight:
            pGround = new CaveExtraDeepEndRight(gridX);
            break;
            
        case GroundType_CaveDeepEndLeft:
            pGround = new CaveDeepEndLeft(gridX);
            break;
        case GroundType_CaveDeepSmall:
            pGround = new CaveDeepSmall(gridX);
            break;
        case GroundType_CaveDeepMedium:
            pGround = new CaveDeepMedium(gridX);
            break;
        case GroundType_CaveDeepLarge:
            pGround = new CaveDeepLarge(gridX);
            break;
        case GroundType_CaveDeepEndRight:
            pGround = new CaveDeepEndRight(gridX);
            break;
            
        case GroundType_CaveEndLeft:
            pGround = new CaveEndLeft(gridX);
            break;
        case GroundType_CaveSmall:
            pGround = new CaveSmall(gridX);
            break;
        case GroundType_CaveMedium:
            pGround = new CaveMedium(gridX);
            break;
        case GroundType_CaveLarge:
            pGround = new CaveLarge(gridX);
            break;
        case GroundType_CaveEndRight:
            pGround = new CaveEndRight(gridX);
            break;
            
        case GroundType_CaveRaisedEndLeft:
            pGround = new CaveRaisedEndLeft(gridX);
            break;
        case GroundType_CaveRaisedSmall:
            pGround = new CaveRaisedSmall(gridX);
            break;
        case GroundType_CaveRaisedMedium:
            pGround = new CaveRaisedMedium(gridX);
            break;
        case GroundType_CaveRaisedLarge:
            pGround = new CaveRaisedLarge(gridX);
            break;
        case GroundType_CaveRaisedEndRight:
            pGround = new CaveRaisedEndRight(gridX);
            break;
            
        case GroundType_GrassWithCaveEndLeft:
            pGround = new GrassWithCaveEndLeft(gridX);
            break;
        case GroundType_GrassWithCaveSmall:
            pGround = new GrassWithCaveSmall(gridX);
            break;
        case GroundType_GrassWithCaveMedium:
            pGround = new GrassWithCaveMedium(gridX);
            break;
        case GroundType_GrassWithCaveLarge:
            pGround = new GrassWithCaveLarge(gridX);
            break;
        case GroundType_GrassWithCaveEndRight:
            pGround = new GrassWithCaveEndRight(gridX);
            break;
            
        case GroundType_GrassWithoutCaveEndLeft:
            pGround = new GrassWithoutCaveEndLeft(gridX);
            break;
        case GroundType_GrassWithoutCaveSmall:
            pGround = new GrassWithoutCaveSmall(gridX);
            break;
        case GroundType_GrassWithoutCaveMedium:
            pGround = new GrassWithoutCaveMedium(gridX);
            break;
        case GroundType_GrassWithoutCaveLarge:
            pGround = new GrassWithoutCaveLarge(gridX);
            break;
        case GroundType_GrassWithoutCaveEndRight:
            pGround = new GrassWithoutCaveEndRight(gridX);
            break;
        
        default:
            assert(false);
    }
    
    return pGround;
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