//
//  ForegroundObject.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/9/16.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#include "ForegroundObject.h"
#include "EntityUtils.h"

ForegroundObject* ForegroundObject::create(int gridX, int gridY, int type)
{
    switch ((ForegroundObjectType)type)
    {
        case ForegroundObjectType_GrassPlatformLeft:
            return new GrassPlatformLeft(gridX, gridY);
        case ForegroundObjectType_GrassPlatformCenter:
            return new GrassPlatformCenter(gridX, gridY);
        case ForegroundObjectType_GrassPlatformRight:
            return new GrassPlatformRight(gridX, gridY);
            
        case ForegroundObjectType_CavePlatformLeft:
            return new CavePlatformLeft(gridX, gridY);
        case ForegroundObjectType_CavePlatformCenter:
            return new CavePlatformCenter(gridX, gridY);
        case ForegroundObjectType_CavePlatformRight:
            return new CavePlatformRight(gridX, gridY);
            
        case ForegroundObjectType_RockLarge:
            return new RockLarge(gridX, gridY);
        case ForegroundObjectType_RockMedium:
            return new RockMedium(gridX, gridY);
        case ForegroundObjectType_RockSmall:
            return new RockSmall(gridX, gridY);
        case ForegroundObjectType_RockSmallCracked:
            return new RockSmallCracked(gridX, gridY);
            
        case ForegroundObjectType_StumpBig:
            return new StumpBig(gridX, gridY);
        case ForegroundObjectType_StumpSmall:
            return new StumpSmall(gridX, gridY);
            
        case ForegroundObjectType_EndSign:
            return new EndSign(gridX, gridY);
            
        case ForegroundObjectType_ThornsLeft:
            return new ThornsLeft(gridX, gridY);
        case ForegroundObjectType_ThornsCenterSmall:
            return new ThornsCenterSmall(gridX, gridY);
        case ForegroundObjectType_ThornsCenterBig:
            return new ThornsCenterBig(gridX, gridY);
        case ForegroundObjectType_ThornsRight:
            return new ThornsRight(gridX, gridY);
            
        case ForegroundObjectType_LogVerticalTall:
            return new LogVerticalTall(gridX, gridY);
        case ForegroundObjectType_LogVerticalShort:
            return new LogVerticalShort(gridX, gridY);
            
        case ForegroundObjectType_JumpSpringLight:
            return new JumpSpringLight(gridX, gridY);
        case ForegroundObjectType_JumpSpringMedium:
            return new JumpSpringMedium(gridX, gridY);
        case ForegroundObjectType_JumpSpringHeavy:
            return new JumpSpringHeavy(gridX, gridY);
            
        case ForegroundObjectType_SpikeGrassSingle:
            return new SpikeGrassSingle(gridX, gridY);
        case ForegroundObjectType_SpikeGrassFour:
            return new SpikeGrassFour(gridX, gridY);
        case ForegroundObjectType_SpikeGrassEight:
            return new SpikeGrassEight(gridX, gridY);
            
        case ForegroundObjectType_SpikeCaveSingle:
            return new SpikeCaveSingle(gridX, gridY);
        case ForegroundObjectType_SpikeCaveFour:
            return new SpikeCaveFour(gridX, gridY);
        case ForegroundObjectType_SpikeCaveEight:
            return new SpikeCaveEight(gridX, gridY);
            
        case ForegroundObjectType_SpikeCaveCeilingSingle:
            return new SpikeCaveCeilingSingle(gridX, gridY);
        case ForegroundObjectType_SpikeCaveCeilingFour:
            return new SpikeCaveCeilingFour(gridX, gridY);
        case ForegroundObjectType_SpikeCaveCeilingEight:
            return new SpikeCaveCeilingEight(gridX, gridY);
            
        case ForegroundObjectType_SpikeWallSingle:
            return new SpikeWallSingle(gridX, gridY);
        case ForegroundObjectType_SpikeWallFour:
            return new SpikeWallFour(gridX, gridY);
        case ForegroundObjectType_SpikeWallEight:
            return new SpikeWallEight(gridX, gridY);
    }
    
    assert(false);
}

ForegroundObject::ForegroundObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType, float boundsX, float boundsY, float boundsWidth, float boundsHeight) : GridLockedPhysicalEntity(gridX, gridY, gridWidth, gridHeight, boundsX, boundsY, boundsWidth, boundsHeight), m_type(type), m_groundSoundType(groundSoundType)
{
    // Empty
}

bool ForegroundObject::isJonLanding(Jon& jon, float deltaTime)
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

bool ForegroundObject::isJonBlockedOnRight(Jon &jon, float deltaTime)
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

bool ForegroundObject::isJonBlockedAbove(Jon& jon, float deltaTime)
{
    return false;
}

bool ForegroundObject::isJonHittingHorizontally(Jon& jon, float deltaTime)
{
    return false;
}

bool ForegroundObject::isJonHittingFromBelow(Jon& jon, float deltaTime)
{
    return false;
}

bool ForegroundObject::canObjectBePlacedOn()
{
    return false;
}

ForegroundObjectType ForegroundObject::getType()
{
    return m_type;
}

GroundSoundType ForegroundObject::getGroundSoundType()
{
    return m_groundSoundType;
}

bool PlatformObject::isJonBlockedOnRight(Jon &jon, float deltaTime)
{
	return false;
}

bool PlatformObject::canObjectBePlacedOn()
{
    return true;
}

void DestructibleObject::update(float deltaTime)
{
    if (m_isDestructing)
    {
        m_fStateTime += deltaTime;
        
        if (m_fStateTime > 0.30f)
        {
            m_isDestructing = false;
            m_isRequestingDeletion = true;
        }
    }
}

bool DestructibleObject::isJonHittingHorizontally(Jon& jon, float deltaTime)
{
    Rectangle& bounds = jon.getBounds();
    Rectangle hittingBounds = Rectangle(bounds.getLeft(), bounds.getLowerLeft().getY() + bounds.getHeight() / 2, bounds.getWidth() * 1.2f, bounds.getHeight());
    
    if (OverlapTester::doRectanglesOverlap(hittingBounds, getBounds()))
    {
        m_isDestructing = true;
        m_fStateTime = 0;
        
        return true;
    }
    
    return false;
}

bool DestructibleObject::isJonHittingFromBelow(Jon& jon, float deltaTime)
{
    Rectangle& bounds = jon.getBounds();
    
    if (OverlapTester::doRectanglesOverlap(bounds, getBounds()))
    {
        m_isDestructing = true;
        m_fStateTime = 0;
        
        return true;
    }
    
    return false;
}

bool DeadlyObject::isJonLanding(Jon& jon, float deltaTime)
{
    if (ForegroundObject::isJonLanding(jon, deltaTime))
    {
        jon.kill();
        
        return true;
    }
    
    return false;
}

bool DeadlyObject::isJonBlockedOnRight(Jon& jon, float deltaTime)
{
    if (ForegroundObject::isJonBlockedOnRight(jon, deltaTime))
    {
        jon.kill();
        
        return true;
    }
    
    return false;
}

bool DeadlyObject::isJonBlockedAbove(Jon& jon, float deltaTime)
{
    float entityVelocityY = jon.getVelocity().getY();
    
    if (entityVelocityY > 0 && OverlapTester::doRectanglesOverlap(jon.getBounds(), getBounds()))
    {
        jon.getPosition().sub(0, jon.getVelocity().getY() * deltaTime);
        jon.updateBounds();
        jon.kill();
        
        return true;
    }
    
    return false;
}

bool LandingDeathObject::isJonLanding(Jon& jon, float deltaTime)
{
    if (ForegroundObject::isJonLanding(jon, deltaTime))
    {
        jon.kill();
    }
    
    return false;
}

bool RunningIntoDeathObject::isJonBlockedOnRight(Jon& jon, float deltaTime)
{
    if (ForegroundObject::isJonBlockedOnRight(jon, deltaTime))
    {
        jon.kill();
    }
    
    return false;
}

bool DeathFromAboveObject::isJonBlockedAbove(Jon& jon, float deltaTime)
{
    float entityVelocityY = jon.getVelocity().getY();
    
    if (entityVelocityY > 0 && OverlapTester::doRectanglesOverlap(jon.getBounds(), getBounds()))
    {
        jon.getPosition().sub(0, jon.getVelocity().getY() * deltaTime);
        jon.updateBounds();
        jon.kill();
    }
    
    return false;
}

void ProvideBoostObject::update(float deltaTime)
{
    if (m_isBoosting)
    {
        m_fStateTime += deltaTime;
        
        if (m_fStateTime > 0.36f)
        {
            m_isBoosting = false;
            m_fStateTime = 0;
        }
    }
}

bool ProvideBoostObject::isJonLanding(Jon& jon, float deltaTime)
{
    if (ForegroundObject::isJonLanding(jon, deltaTime))
    {
        float itemTop = getBounds().getTop();
        jon.getPosition().setY(itemTop + jon.getBounds().getHeight() / 2 * 1.01f);
        
        jon.triggerBoost(m_fBoostVelocity);
        m_isBoosting = true;
        m_fStateTime = 0;
    }
    
    return false;
}
