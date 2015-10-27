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

Ground* Ground::create(float x, float y, int type)
{
    Ground* pGround;
    
    switch ((GroundType)type)
    {
        case GroundType_Grass_with_Cave_Large:
            pGround = new GroundGrassWithCaveLarge(x);
            break;
        case GroundType_Grass_with_Cave_Medium:
            pGround = new GroundGrassWithCaveMedium(x);
            break;
        case GroundType_Grass_with_Cave_Small:
            pGround = new GroundGrassWithCaveSmall(x);
            break;
        case GroundType_Grass_with_Cave_End_Left:
            pGround = new GroundGrassWithCaveEndLeft(x);
            break;
        case GroundType_Grass_with_Cave_End_Right:
            pGround = new GroundGrassWithCaveEndRight(x);
            break;
            
        case GroundType_Grass_without_Cave_Large:
            pGround = new GroundGrassWithoutCaveLarge(x);
            break;
        case GroundType_Grass_without_Cave_Medium:
            pGround = new GroundGrassWithoutCaveMedium(x);
            break;
        case GroundType_Grass_without_Cave_Small:
            pGround = new GroundGrassWithoutCaveSmall(x);
            break;
        case GroundType_Grass_without_Cave_End_Left:
            pGround = new GroundGrassWithoutCaveEndLeft(x);
            break;
        case GroundType_Grass_without_Cave_End_Right:
            pGround = new GroundGrassWithoutCaveEndRight(x);
            break;
            
        case GroundType_Cave_Large:
            pGround = new GroundCaveLarge(x);
            break;
        case GroundType_Cave_Medium:
            pGround = new GroundCaveMedium(x);
            break;
        case GroundType_Cave_Small:
            pGround = new GroundCaveSmall(x);
            break;
        case GroundType_Cave_End_Left:
            pGround = new GroundCaveEndLeft(x);
            break;
        case GroundType_Cave_End_Right:
            pGround = new GroundCaveEndRight(x);
            break;
            
        case GroundType_Cave_Raised_Large:
            pGround = new GroundCaveRaisedLarge(x);
            break;
        case GroundType_Cave_Raised_Medium:
            pGround = new GroundCaveRaisedMedium(x);
            break;
        case GroundType_Cave_Raised_Small:
            pGround = new GroundCaveRaisedSmall(x);
            break;
        case GroundType_Cave_Raised_End_Left:
            pGround = new GroundCaveRaisedEndLeft(x);
            break;
        case GroundType_Cave_Raised_End_Right:
        default:
            pGround = new GroundCaveRaisedEndRight(x);
            break;
    }
    
    if (type != GroundType_Grass_with_Cave_Large
        && type != GroundType_Grass_with_Cave_Medium
        && type != GroundType_Grass_with_Cave_Small
        && type != GroundType_Grass_with_Cave_End_Left
        && type != GroundType_Grass_with_Cave_End_Right)
    {
        EntityUtils::applyAnchor(*pGround, ANCHOR_BOTTOM);
        pGround->updateBounds();
    }
    
    return pGround;
}

Ground::Ground(float x, float y, float width, float height, float boundsHeightFactor, GroundType type) : PhysicalEntity(x, y, width, height), m_type(type), m_fBoundsHeightFactor(boundsHeightFactor)
{
    updateBounds();
}

void Ground::updateBounds()
{
    m_bounds->setHeight(getHeight());
    
    PhysicalEntity::updateBounds();
    
    m_bounds->setHeight(getHeight() * m_fBoundsHeightFactor);
}

bool Ground::isForegroundMore()
{
    return m_type == GroundType_Cave_Raised_Large
    || m_type == GroundType_Cave_Raised_Medium
    || m_type == GroundType_Cave_Raised_Small
    || m_type == GroundType_Cave_Raised_End_Left
    || m_type == GroundType_Cave_Raised_End_Right;
}

GroundType Ground::getEnumType()
{
    return m_type;
}

int Ground::getType()
{
    return m_type;
}