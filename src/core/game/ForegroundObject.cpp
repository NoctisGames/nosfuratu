//
//  ForegroundObject.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/9/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "ForegroundObject.h"
#include "EntityUtils.h"
#include "GameConstants.h"
#include "Assets.h"
#include "Game.h"

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
            
        case ForegroundObjectType_JumpSpringLightFlush:
            return new JumpSpringLightFlush(gridX, gridY);
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
            
        case ForegroundObjectType_SpikeStar:
            return new SpikeStar(gridX, gridY);
            
        case ForegroundObjectType_VerticalSaw:
            return new VerticalSaw(gridX);
            
        case ForegroundObjectType_GiantTree:
            return new GiantTree(gridX, gridY);
        case ForegroundObjectType_GiantShakingTree:
            return new GiantShakingTree(gridX, gridY);
        case ForegroundObjectType_GiantPerchTree:
            return new GiantPerchTree(gridX, gridY);
            
        case ForegroundObjectType_SpikeTower:
            return new SpikeTower(gridX);
        case ForegroundObjectType_SpikeTowerBg:
            return new SpikeTowerBg(gridX);
            
        case ForegroundObjectType_Boulder:
            return new Boulder(gridX, gridY);
    }
    
    assert(false);
}

ForegroundObject::ForegroundObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType, float boundsX, float boundsY, float boundsWidth, float boundsHeight) : GridLockedPhysicalEntity(gridX, gridY, gridWidth, gridHeight, boundsX, boundsY, boundsWidth, boundsHeight), m_type(type), m_groundSoundType(groundSoundType), m_game(nullptr)
{
    // Empty
}

void ForegroundObject::updateBounds()
{
    GridLockedPhysicalEntity::updateBounds();
}

bool ForegroundObject::isEntityLanding(PhysicalEntity* entity, float deltaTime)
{
    return isEntityLanding(entity, getMainBounds(), deltaTime);
}

bool ForegroundObject::isJonBlockedOnRight(Jon &jon, float deltaTime)
{
    return isJonBlockedOnRight(jon, getMainBounds(), deltaTime);
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

void ForegroundObject::setGame(Game* game)
{
    m_game = game;
}

#pragma mark protected

bool ForegroundObject::isEntityLanding(PhysicalEntity* entity, Rectangle& bounds, float deltaTime)
{
    float entityVelocityY = entity->getVelocity().getY();
    
    if (entityVelocityY <= 0
        && entity->getPosition().getX() > getMainBounds().getLeft()
        && (entity->getMainBounds().getBottom() + 0.01f) > getMainBounds().getBottom())
    {
        if (OverlapTester::doRectanglesOverlap(entity->getMainBounds(), bounds))
        {
            float itemTop = bounds.getTop();
            
            entity->getPosition().setY(itemTop + entity->getMainBounds().getHeight() / 2 * .99f);
            entity->updateBounds();
            
            Jon *jon;
            if ((jon = dynamic_cast<Jon *>(entity)))
            {
                jon->setGroundSoundType(getGroundSoundType());
            }
            
            return true;
        }
    }
    
    return false;
}

bool ForegroundObject::isJonBlockedOnRight(Jon &jon, Rectangle& bounds, float deltaTime)
{
    if (OverlapTester::doRectanglesOverlap(jon.getMainBounds(), bounds))
    {
        float entityVelocityX = jon.getVelocity().getX();
        float entityBottom = jon.getMainBounds().getLowerLeft().getY();
        float entityRight = jon.getMainBounds().getRight();
        float entityXDelta = fabsf(entityVelocityX * deltaTime);
        
        float itemTop = bounds.getTop();
        float itemTopReq = itemTop * 0.99f;
        
        float itemLeft = bounds.getLeft();
        float padding = itemLeft * .01f;
        padding += entityXDelta;
        float itemLeftReq = itemLeft + padding;
        
        if (entityRight <= itemLeftReq && entityBottom < itemTopReq)
        {
            jon.getPosition().setX(itemLeft - jon.getMainBounds().getWidth() / 2 * 1.01f);
            jon.updateBounds();
            
            return true;
        }
    }
    
    return false;
}

#pragma mark subclasses

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
    Rectangle& bounds = jon.getMainBounds();
    Rectangle hittingBounds = Rectangle(bounds.getLeft(), bounds.getLowerLeft().getY() + bounds.getHeight() / 2, bounds.getWidth() * 1.2f, bounds.getHeight());
    
    if (OverlapTester::doRectanglesOverlap(hittingBounds, getMainBounds()))
    {
        m_isDestructing = true;
        m_fStateTime = 0;
        
        return true;
    }
    
    return false;
}

bool DestructibleObject::isJonHittingFromBelow(Jon& jon, float deltaTime)
{
    Rectangle& bounds = jon.getMainBounds();
    
    if (OverlapTester::doRectanglesOverlap(bounds, getMainBounds()))
    {
        m_isDestructing = true;
        m_fStateTime = 0;
        
        return true;
    }
    
    return false;
}

bool DeadlyObject::isEntityLanding(PhysicalEntity* entity, float deltaTime)
{
    float entityY = entity->getPosition().getY();
    
    if (ForegroundObject::isEntityLanding(entity, deltaTime))
    {
        Jon *jon;
        if ((jon = dynamic_cast<Jon *>(entity)))
        {
            jon->kill();
            
            if (entityY > getMainBounds().getBottom())
            {
                entity->getPosition().setY(entityY);
            }
        }
        
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
    
    if (entityVelocityY > 0 && OverlapTester::doRectanglesOverlap(jon.getMainBounds(), getMainBounds()))
    {
        jon.getPosition().sub(0, jon.getVelocity().getY() * deltaTime);
        jon.updateBounds();
        jon.kill();
    }
    
    return false;
}

bool LandingDeathObject::isEntityLanding(PhysicalEntity* entity, float deltaTime)
{
    if (ForegroundObject::isEntityLanding(entity, deltaTime))
    {
        Jon *jon;
        if ((jon = dynamic_cast<Jon *>(entity)))
        {
            jon->kill();
        }
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
    
    if (entityVelocityY > 0 && OverlapTester::doRectanglesOverlap(jon.getMainBounds(), getMainBounds()))
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

bool ProvideBoostObject::isEntityLanding(PhysicalEntity* entity, float deltaTime)
{
    if (ForegroundObject::isEntityLanding(entity, deltaTime))
    {
        float itemTop = getMainBounds().getTop();
        entity->getPosition().setY(itemTop + entity->getMainBounds().getHeight() / 2 * 1.01f);
        
        Jon *jon;
        if ((jon = dynamic_cast<Jon *>(entity)))
        {
            jon->triggerBoost(m_fBoostVelocity);
        }
        
        m_isBoosting = true;
        m_fStateTime = 0;
    }
    
    return false;
}

bool JumpSpringLightFlush::isJonBlockedOnRight(Jon &jon, float deltaTime)
{
    return false;
}

void GiantShakingTree::update(float deltaTime)
{
    if (m_isShaking)
    {
        ForegroundObject::update(deltaTime);
        
        if (m_fStateTime > 0.70f)
        {
            m_fStateTime = 0.0f;
            m_isShaking = false;
        }
    }
}

void GiantShakingTree::triggerHit()
{
    m_isShaking = true;
}

void ExtraForegroundObject::update(float deltaTime)
{
    ForegroundObject::update(deltaTime);
    
    m_shadow->update(deltaTime);
    m_shadow->getPosition().set(getPosition());
    m_shadow->updateBounds();
}

void SpikeTower::updateBounds()
{
    ForegroundObject::updateBounds();
    
    Rectangle& bounds = getBounds().at(1);
    
    bounds.setWidth(getWidth());
    bounds.setHeight(getHeight());
    
    Vector2D &lowerLeft = bounds.getLowerLeft();
    lowerLeft.set(m_position->getX() - bounds.getWidth() / 2, m_position->getY() - bounds.getHeight() / 2);
    
    bounds.getLowerLeft().add(getWidth() * 0.0f, getHeight() * 0.79910714285714f);
    bounds.setWidth(getWidth() * 1.0f);
    bounds.setHeight(getHeight() * 0.20089285714286f);
}

bool SpikeTower::isEntityLanding(PhysicalEntity* entity, float deltaTime)
{
    bool ret = false;
    
    Jon *jon;
    if ((jon = dynamic_cast<Jon *>(entity)))
    {
        jon->getMainBounds().setAngle(jon->getAbilityState() == ABILITY_GLIDE ? 90 : 0);
        
        if (ForegroundObject::isEntityLanding(jon, deltaTime)
            || ForegroundObject::isEntityLanding(jon, getBounds().at(1), deltaTime))
        {
            Jon *jon;
            if ((jon = dynamic_cast<Jon *>(entity)))
            {
                jon->kill();
            }
            
            ret = true;
        }
        
        jon->getMainBounds().setAngle(0);
    }
    
    return ret;
}

bool SpikeTower::isJonBlockedOnRight(Jon& jon, float deltaTime)
{
    bool ret = false;
    
    jon.getMainBounds().setAngle(jon.getAbilityState() == ABILITY_GLIDE ? 90 : 0);
    
    if (ForegroundObject::isJonBlockedOnRight(jon, deltaTime)
        || ForegroundObject::isJonBlockedOnRight(jon, getBounds().at(1), deltaTime))
    {
        jon.kill();
        
        ret = true;
    }
    
    jon.getMainBounds().setAngle(0);
    
    return ret;
}

void VerticalSaw::updateBounds()
{
    ForegroundObject::updateBounds();
    
    Rectangle& camBounds = *m_game->getCameraBounds();
    
    if (camBounds.getWidth() > CAM_WIDTH)
    {
        return;
    }
    
    if (OverlapTester::doRectanglesOverlap(camBounds, getMainBounds()))
    {
        if (!m_isOnScreen)
        {
            m_isOnScreen = true;
            
            Assets::getInstance()->addSoundIdToPlayQueue(SOUND_SAW_GRIND);
        }
    }
    else if (m_isOnScreen)
    {
        m_isOnScreen = false;
        
        Assets::getInstance()->forceAddSoundIdToPlayQueue(STOP_SOUND_SAW_GRIND);
    }
}
