//
//  Ground.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/5/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "Ground.h"
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

Ground::Ground(int gridX, int gridY, int gridWidth, int gridHeight, float boundsY, float boundsHeight, GroundType type, GroundSoundType groundSoundType) : GridLockedPhysicalEntity(gridX, gridY, gridWidth, gridHeight, 0, boundsY, 1, boundsHeight), m_type(type), m_groundSoundType(groundSoundType)
{
    updateBounds();
}

bool Ground::isJonLanding(Jon& jon, float deltaTime)
{
    float jonVelocityY = jon.getVelocity().getY();
    
    if (jonVelocityY <= 0)
    {
        if (OverlapTester::doRectanglesOverlap(jon.getBounds(), getBounds()))
        {
            float jonLowerLeftY = jon.getBounds().getLowerLeft().getY();
            float jonYDelta = fabsf(jonVelocityY * deltaTime);
            
            float itemTop = getBounds().getTop();
            float padding = itemTop * .01f;
            padding += jonYDelta;
            float itemTopReq = itemTop - padding;
            
            if (jonLowerLeftY >= itemTopReq)
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

bool Ground::isJonBlockedOnRight(Jon &jon, float deltaTime)
{
    if (OverlapTester::doRectanglesOverlap(jon.getBounds(), getBounds()))
    {
        float entityVelocityX = jon.getVelocity().getX();
        float entityBottom = jon.getBounds().getLowerLeft().getY();
        float entityRight = jon.getBounds().getRight();
        float entityXDelta = fabsf(entityVelocityX * deltaTime);
        
        float itemTop = getBounds().getTop();
        float itemTopReq = itemTop * 0.99f;
        
        float itemLeft = getBounds().getLeft();
        float padding = itemLeft * .01f;
        padding += entityXDelta;
        float itemLeftReq = itemLeft + padding;
        
        if (entityRight <= itemLeftReq && entityBottom < itemTopReq)
        {
            jon.getPosition().setX(itemLeft - jon.getBounds().getWidth() / 2 * 1.01f);
            jon.updateBounds();
            
            return true;
        }
    }
    
    return false;
}

bool Ground::isJonBlockedAbove(Jon &jon, float deltaTime)
{
    float entityVelocityY = jon.getVelocity().getY();
    
    if (entityVelocityY > 0 && OverlapTester::doRectanglesOverlap(jon.getBounds(), getBounds()))
    {
        float entityLeft = jon.getBounds().getLeft();
        float itemLeft = getBounds().getLeft();
        
        if (itemLeft < entityLeft)
        {
            jon.getPosition().sub(0, jon.getVelocity().getY() * deltaTime);
            jon.updateBounds();
            
            return true;
        }
    }
    
    return false;
}

bool Ground::canObjectBePlacedOn()
{
    return true;
}

bool Ground::canObjectBePlacedUnder()
{
    return m_fBoundsY > 0;
}

GroundType Ground::getType()
{
    return m_type;
}

GroundSoundType Ground::getGroundSoundType()
{
    return m_groundSoundType;
}