//
//  ForegroundCoverObject.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 11/23/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "ForegroundCoverObject.h"

#include "Jon.h"
#include "Game.h"

#include "EntityUtils.h"
#include "GameConstants.h"
#include "NGRect.h"

ForegroundCoverObject* ForegroundCoverObject::create(int gridX, int gridY, int type)
{
    ForegroundCoverObjectType fcot = (ForegroundCoverObjectType)type;
    switch (fcot)
    {
        case ForegroundCoverObjectType_Tree:
            return new ForegroundCoverObject(gridX, 85, 28, 51, fcot);
        case ForegroundCoverObjectType_Plant:
            return new ForegroundCoverObject(gridX, 92, 21, 24, fcot);
        case ForegroundCoverObjectType_Bush:
            return new ForegroundCoverObject(gridX, 92, 25, 15, fcot);
        case ForegroundCoverObjectType_Ferns:
            return new ForegroundCoverObject(gridX, 92, 33, 11, fcot);
        case ForegroundCoverObjectType_Wall:
            return new ForegroundCoverObject(gridX, gridY, 16, 16, fcot);
        case ForegroundCoverObjectType_Wall_Bottom:
            return new ForegroundCoverObject(gridX, 96, 16, 16, fcot);
        case ForegroundCoverObjectType_Wall_Window:
            return new ForegroundCoverObject(gridX, gridY, 16, 16, fcot);
        case ForegroundCoverObjectType_Wall_Window_Bottom:
            return new ForegroundCoverObject(gridX, 96, 16, 16, fcot);
        case ForegroundCoverObjectType_Roof_Side_Left:
            return new LandableForegroundCoverObject(gridX, gridY, 16, 4, fcot, GROUND_SOUND_WOOD, 0, 0, 1, 0.5f);
        case ForegroundCoverObjectType_Roof_Side_Right:
            return new LandableForegroundCoverObject(gridX, gridY, 16, 4, fcot, GROUND_SOUND_WOOD, 0, 0, 1, 0.5f);
        case ForegroundCoverObjectType_Roof_Plain:
            return new LandableForegroundCoverObject(gridX, gridY, 16, 6, fcot, GROUND_SOUND_WOOD, 0, 0, 1, 0.64f);
        case ForegroundCoverObjectType_Roof_Chimney:
            return new LandableForegroundCoverObject(gridX, gridY, 16, 6, fcot, GROUND_SOUND_WOOD, 0, 0, 1, 0.64f);
    }
    
    assert(false);
}

ForegroundCoverObject::ForegroundCoverObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundCoverObjectType type, GroundSoundType groundSoundType, float boundsX, float boundsY, float boundsWidth, float boundsHeight) : GridLockedPhysicalEntity(gridX, gridY, gridWidth, gridHeight, boundsX, boundsY, boundsWidth, boundsHeight), m_type(type), m_groundSoundType(groundSoundType), m_game(nullptr), m_color(1, 1, 1, 1)
{
    // Empty
}

bool ForegroundCoverObject::isEntityLanding(PhysicalEntity* entity, float deltaTime)
{
    return false;
}

int ForegroundCoverObject::getEntityLandingPriority()
{
    return 0;
}

bool ForegroundCoverObject::isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime)
{
    return false;
}

bool ForegroundCoverObject::isEntityBlockedOnLeft(PhysicalEntity* entity, float deltaTime)
{
    return false;
}

bool ForegroundCoverObject::isJonBlockedAbove(Jon& jon, float deltaTime)
{
    return false;
}

bool ForegroundCoverObject::isJonHittingHorizontally(Jon& jon, float deltaTime)
{
    return false;
}

bool ForegroundCoverObject::isJonHittingFromBelow(Jon& jon, float deltaTime)
{
    return false;
}

bool ForegroundCoverObject::canObjectBePlacedOn()
{
    return false;
}

ForegroundCoverObjectType ForegroundCoverObject::getType()
{
    return m_type;
}

GroundSoundType ForegroundCoverObject::getGroundSoundType()
{
    return m_groundSoundType;
}

void ForegroundCoverObject::setGame(Game* game)
{
    m_game = game;
}

Color ForegroundCoverObject::getColor()
{
    return m_color;
}

LandableForegroundCoverObject::LandableForegroundCoverObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundCoverObjectType type, GroundSoundType groundSoundType, float boundsX, float boundsY, float boundsWidth, float boundsHeight) : ForegroundCoverObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType, boundsX, boundsY, boundsWidth, boundsHeight)
{
    // Empty
}

bool LandableForegroundCoverObject::isEntityLanding(PhysicalEntity* entity, float deltaTime)
{
    float entityVelocityY = entity->getVelocity().getY();
    
    if (entityVelocityY <= 0
        && entity->getMainBounds().getRight() > getMainBounds().getLeft())
    {
        float entityYDelta = fabsf(entityVelocityY * deltaTime);
        
        NGRect tempBounds = NGRect(getMainBounds().getLeft(), getMainBounds().getBottom(), getMainBounds().getWidth(), getMainBounds().getHeight());
        
        tempBounds.setHeight(tempBounds.getHeight() + entityYDelta);
        
        if (OverlapTester::doNGRectsOverlap(entity->getMainBounds(), tempBounds))
        {
            float jonLowerLeftY = entity->getMainBounds().getBottom();
            
            float itemTop = tempBounds.getTop();
            float padding = itemTop * .01f;
            padding += entityYDelta;
            float itemTopReq = itemTop - padding;
            
            if (jonLowerLeftY >= itemTopReq)
            {
                entity->placeOn(getMainBounds().getTop());
                
                Jon *jon = nullptr;
                if (entity->getRTTI().derivesFrom(Jon::rtti))
                {
                    jon = reinterpret_cast<Jon *>(entity);
                    jon->setGroundSoundType(getGroundSoundType());
                }
                
                return true;
            }
        }
    }
    
    return false;
}

bool LandableForegroundCoverObject::isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime)
{
    if (OverlapTester::doNGRectsOverlap(entity->getMainBounds(), getMainBounds()))
    {
        float entityVelocityX = entity->getVelocity().getX();
        float entityBottom = entity->getMainBounds().getBottom();
        float entityRight = entity->getMainBounds().getRight();
        float entityXDelta = entityVelocityX * deltaTime;
        
        float itemTop = getMainBounds().getTop();
        float itemTopReq = itemTop * 0.99f;
        
        float itemLeft = getMainBounds().getLeft();
        float padding = itemLeft * .01f;
        padding += entityXDelta;
        float itemLeftReq = itemLeft + padding;
        
        if (entityRight <= itemLeftReq && entityBottom < itemTopReq)
        {
            entity->getPosition().setX(itemLeft - entity->getMainBounds().getWidth() / 2 * 1.01f);
            entity->updateBounds();
            
            return true;
        }
    }
    
    return false;
}

bool LandableForegroundCoverObject::isEntityBlockedOnLeft(PhysicalEntity* entity, float deltaTime)
{
    if (OverlapTester::doNGRectsOverlap(entity->getMainBounds(), getMainBounds()))
    {
        float entityVelocityX = entity->getVelocity().getX();
        float entityBottom = entity->getMainBounds().getBottom();
        float entityLeft = entity->getMainBounds().getLeft();
        float entityXDelta = entityVelocityX * deltaTime;
        
        float itemTop = getMainBounds().getTop();
        float itemTopReq = itemTop * 0.99f;
        
        float itemRight = getMainBounds().getRight();
        float padding = itemRight * .01f;
        padding -= entityXDelta;
        float itemRightReq = itemRight - padding;
        
        if (entityLeft >= itemRightReq && entityBottom < itemTopReq)
        {
            entity->getPosition().setX(itemRight + entity->getMainBounds().getWidth() / 2 * 1.01f);
            entity->updateBounds();
            
            return true;
        }
    }
    
    return false;
}

bool LandableForegroundCoverObject::isJonBlockedAbove(Jon &jon, float deltaTime)
{
    float entityVelocityY = jon.getVelocity().getY();
    
    if (entityVelocityY > 0 && OverlapTester::doNGRectsOverlap(jon.getMainBounds(), getMainBounds()))
    {
        float entityLeft = jon.getMainBounds().getLeft();
        float itemLeft = getMainBounds().getLeft();
        
        if (itemLeft < entityLeft)
        {
            jon.getPosition().sub(0, jon.getVelocity().getY() * deltaTime);
            jon.updateBounds();
            
            return true;
        }
    }
    
    return false;
}

RTTI_IMPL(ForegroundCoverObject, GridLockedPhysicalEntity);
RTTI_IMPL(LandableForegroundCoverObject, ForegroundCoverObject);
