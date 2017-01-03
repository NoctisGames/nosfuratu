//
//  ForegroundObject.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/9/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "ForegroundObject.h"

#include "EntityUtils.h"
#include "Assets.h"
#include "Game.h"

ForegroundObject* ForegroundObject::create(int gridX, int gridY, int type)
{
    ForegroundObjectType fot = (ForegroundObjectType)type;
    switch (fot)
    {
        case ForegroundObjectType_GrassPlatformLeft:
            return new PlatformObject(gridX, gridY, 4, 6, fot, GROUND_SOUND_GRASS, 0, 0, 1, 0.83333333333333f);
        case ForegroundObjectType_GrassPlatformCenter:
            return new PlatformObject(gridX, gridY, 14, 6, fot, GROUND_SOUND_GRASS, 0, 0, 1, 0.83333333333333f);
        case ForegroundObjectType_GrassPlatformRight:
            return new PlatformObject(gridX, gridY, 4, 6, fot, GROUND_SOUND_GRASS, 0, 0, 1, 0.83333333333333f);
            
        case ForegroundObjectType_CavePlatformLeft:
            return new PlatformObject(gridX, gridY, 4, 6, fot, GROUND_SOUND_CAVE, 0, 0, 1, 0.83333333333333f);
        case ForegroundObjectType_CavePlatformCenter:
            return new PlatformObject(gridX, gridY, 14, 6, fot, GROUND_SOUND_CAVE, 0, 0, 1, 0.83333333333333f);
        case ForegroundObjectType_CavePlatformRight:
            return new PlatformObject(gridX, gridY, 4, 6, fot, GROUND_SOUND_CAVE, 0, 0, 1, 0.83333333333333f);
            
        case ForegroundObjectType_RockLarge:
            return new ForegroundObject(gridX, gridY, 44, 32, fot, GROUND_SOUND_CAVE, 0.03551136363636f, 0.0078125f, 0.77840909090909f, 0.96875f);
        case ForegroundObjectType_RockMedium:
            return new ForegroundObject(gridX, gridY, 24, 24, fot, GROUND_SOUND_CAVE, 0.0625f, 0.015625f, 0.7734375f, 0.73958333333333f);
        case ForegroundObjectType_RockSmall:
        case ForegroundObjectType_RockSmallCracked:
            return new ForegroundObject(gridX, gridY, 24, 23, fot, GROUND_SOUND_CAVE, 0.328125f, 0.20923913043478f, 0.328125f, 0.27717391304348f);
            
        case ForegroundObjectType_StumpBig:
            return new ForegroundObject(gridX, gridY, 16, 18, fot, GROUND_SOUND_GRASS, 0.19140625f, 0.03125f, 0.5f, 0.94444444444444f);
        case ForegroundObjectType_StumpSmall:
            return new ForegroundObject(gridX, gridY, 20, 17, fot, GROUND_SOUND_GRASS, 0.215625f, 0.07352941176471f, 0.375f, 0.73897058823529f);
            
        case ForegroundObjectType_EndSign:
            return new EndSign(gridX, gridY, 7, 10, fot);
            
        case ForegroundObjectType_ThornsLeft:
            return new DeadlyObject(gridX, gridY, 5, 7, fot, GROUND_SOUND_NONE, 0, 0, 1, 0.8f);
        case ForegroundObjectType_ThornsCenterSmall:
            return new DeadlyObject(gridX, gridY, 11, 7, fot, GROUND_SOUND_NONE, 0, 0, 1, 0.8f);
        case ForegroundObjectType_ThornsCenterBig:
            return new DeadlyObject(gridX, gridY, 22, 7, fot, GROUND_SOUND_NONE, 0, 0, 1, 0.8f);
        case ForegroundObjectType_ThornsRight:
            return new DeadlyObject(gridX, gridY, 5, 7, fot, GROUND_SOUND_NONE, 0, 0, 1, 0.8f);
            
        case ForegroundObjectType_LogVerticalTall:
            return new ForegroundObject(gridX, gridY, 8, 11, fot, GROUND_SOUND_GRASS, 0.1484375f, 0.02840909090909f, 0.5f, 0.92613636363636f);
        case ForegroundObjectType_LogVerticalShort:
            return new ForegroundObject(gridX, gridY, 8, 6, fot, GROUND_SOUND_GRASS, 0.1462f, 0.05208333333333f, 0.4453125f, 0.875f);
            
        case ForegroundObjectType_JumpSpringLightFlush:
            return new JumpSpringLightFlush(gridX, gridY, 10, 7, fot, GROUND_SOUND_NONE, 0, 0.033f, 1, 0.1f, 18.0f);
        case ForegroundObjectType_JumpSpringLight:
            return new ProvideBoostObject(gridX, gridY, 6, 5, fot, GROUND_SOUND_NONE, 0, 0, 1, 0.525f, 18.0f);
        case ForegroundObjectType_JumpSpringMedium:
            return new ProvideBoostObject(gridX, gridY, 17, 9, fot, GROUND_SOUND_NONE, 0, 0.20138888888889f, 0.6f, 0.33333333333333f, 24.0f);
        case ForegroundObjectType_JumpSpringHeavy:
            return new ProvideBoostObject(gridX, gridY, 17, 14, fot, GROUND_SOUND_NONE, 0, 0, 0.6f, 0.52678571428571f, 32.0f);
            
        case ForegroundObjectType_SpikeGrassSingle:
            return new LandingDeathObject(gridX, gridY, 6, 6, fot, 0.1f, 0.04166666666667f, 0.8f, 0.85833333333333f);
        case ForegroundObjectType_SpikeGrassFour:
            return new LandingDeathObject(gridX, gridY, 18, 6, fot, 0.1f, 0.04166666666667f, 0.8f, 0.85833333333333f);
        case ForegroundObjectType_SpikeGrassEight:
            return new LandingDeathObject(gridX, gridY, 34, 6, fot, 0.1f, 0.04166666666667f, 0.8f, 0.85833333333333f);
            
        case ForegroundObjectType_SpikeCaveSingle:
            return new LandingDeathObject(gridX, gridY, 6, 6, fot, 0.1f, 0.04166666666667f, 0.8f, 0.85833333333333f);
        case ForegroundObjectType_SpikeCaveFour:
            return new LandingDeathObject(gridX, gridY, 18, 6, fot, 0.1f, 0.04166666666667f, 0.8f, 0.85833333333333f);
        case ForegroundObjectType_SpikeCaveEight:
            return new LandingDeathObject(gridX, gridY, 34, 6, fot, 0.1f, 0.04166666666667f, 0.8f, 0.85833333333333f);
            
        case ForegroundObjectType_SpikeCaveCeilingSingle:
            return new DeathFromAboveObject(gridX, gridY, 6, 6, fot, 0.1f, 0.0f, 0.8f, 0.9f);
        case ForegroundObjectType_SpikeCaveCeilingFour:
            return new DeathFromAboveObject(gridX, gridY, 18, 6, fot, 0.1f, 0.0f, 0.8f, 0.9f);
        case ForegroundObjectType_SpikeCaveCeilingEight:
            return new DeathFromAboveObject(gridX, gridY, 34, 6, fot, 0.1f, 0.0f, 0.8f, 0.9f);
            
        case ForegroundObjectType_SpikeWallSingle:
            return new RunningIntoDeathObject(gridX, gridY, 6, 4, fot);
        case ForegroundObjectType_SpikeWallFour:
            return new RunningIntoDeathObject(gridX, gridY, 6, 16, fot);
        case ForegroundObjectType_SpikeWallEight:
            return new RunningIntoDeathObject(gridX, gridY, 6, 34, fot);
            
        case ForegroundObjectType_SpikeStar:
            return new DeadlyObject(gridX, gridY, 14, 14, fot, GROUND_SOUND_NONE, 0.18f, 0.18f, 0.64f, 0.64f);
            
        case ForegroundObjectType_VerticalSaw:
            return new VerticalSaw(gridX, 95, 15, 33, fot, GROUND_SOUND_NONE, 0.3f, 0.33333333333333f, 0.3f, 0.66666666666667f);
            
        case ForegroundObjectType_GiantTree:
            return new ForegroundObject(gridX, gridY, 68, 65, fot, GROUND_SOUND_NONE, 0.47265625f, 0.009765625f, 0.15625f, 0.990234375f);
        case ForegroundObjectType_GiantShakingTree:
            return new GiantShakingTree(gridX, gridY, 68, 65, fot, GROUND_SOUND_NONE, 0.47265625f, 0.009765625f, 0.15625f, 0.990234375f);
        case ForegroundObjectType_GiantPerchTree:
            return new ForegroundObject(gridX, gridY, 68, 65, fot, GROUND_SOUND_NONE, 0, 0.009765625f, 1, 0.990234375f);
            
        case ForegroundObjectType_SpikeTower:
            return new SpikeTower(gridX, 95, 32, 56, fot, ForegroundObjectType_SpikeTowerBg, GROUND_SOUND_NONE, 0, 0, 1, 0.54017857142857f);
        case ForegroundObjectType_SpikeTowerBg:
            return new ForegroundObject(gridX, 95, 32, 56, fot);
            
        case ForegroundObjectType_SpikedBallRollingLeft:
            return new SpikedBallRollingLeft(gridX, gridY, 32, 30, fot, GROUND_SOUND_NONE, 0.1f, 0.1f, 0.8f, 0.8f);
        case ForegroundObjectType_SpikedBallRollingRight:
            return new SpikedBallRollingRight(gridX, gridY, 32, 30, fot, GROUND_SOUND_NONE, 0.1f, 0.1f, 0.8f, 0.8f);
            
        case ForegroundObjectType_SpikedBallChain:
            return new SpikedBallChain(gridX, gridY, 101, 82, fot, GROUND_SOUND_NONE, 0.72f, 0, 0.14f, 0.25f);
        case ForegroundObjectType_SpikedBall:
            return new SpikedBall(gridX, gridY, 32, 30, fot, GROUND_SOUND_NONE, 0, 0.4f, 1, 0.8f);
            
        case ForegroundObjectType_MetalGrassPlatform:
            return new PlatformObject(gridX, gridY, 16, 6, fot, GROUND_SOUND_GRASS, 0, 0, 1, 0.89583333333333f);
        case ForegroundObjectType_MetalGrassPlatformLeft:
            return new PlatformObject(gridX, gridY, 4, 6, fot, GROUND_SOUND_GRASS, 0, 0, 1, 0.89583333333333f);
        case ForegroundObjectType_MetalGrassPlatformCenter:
            return new PlatformObject(gridX, gridY, 16, 6, fot, GROUND_SOUND_GRASS, 0, 0, 1, 0.89583333333333f);
        case ForegroundObjectType_MetalGrassPlatformRight:
            return new PlatformObject(gridX, gridY, 4, 6, fot, GROUND_SOUND_GRASS, 0, 0, 1, 0.89583333333333f);
            
        case ForegroundObjectType_WoodPlatform:
            return new PlatformObject(gridX, gridY, 16, 4, fot, GROUND_SOUND_WOOD, 0, 0, 1, 0.875f);
        case ForegroundObjectType_WoodBoxTop:
            return new ForegroundObject(gridX, gridY, 16, 16, fot, GROUND_SOUND_WOOD, 0, 0, 1, 0.96875f);
        case ForegroundObjectType_WoodBox:
            return new ForegroundObject(gridX, gridY, 16, 16, fot, GROUND_SOUND_WOOD);
            
        case ForegroundObjectType_GreenThornsLeft:
            return new DeadlyObject(gridX, gridY, 8, 8, fot, GROUND_SOUND_NONE, 0, 0, 1, 0.8f);
        case ForegroundObjectType_GreenThornsCenterSmall:
            return new DeadlyObject(gridX, gridY, 12, 8, fot, GROUND_SOUND_NONE, 0, 0, 1, 0.8f);
        case ForegroundObjectType_GreenThornsCenterBig:
            return new DeadlyObject(gridX, gridY, 24, 8, fot, GROUND_SOUND_NONE, 0, 0, 1, 0.8f);
        case ForegroundObjectType_GreenThornsRight:
            return new DeadlyObject(gridX, gridY, 8, 8, fot, GROUND_SOUND_NONE, 0, 0, 1, 0.8f);
            
        case ForegroundObjectType_Logs:
            return new ForegroundObject(gridX, gridY, 37, 10, fot, GROUND_SOUND_GRASS, 0, 0, 1, 0.9375f);
            
        case ForegroundObjectType_Stone_Bottom:
            return new BlockingObject(gridX, gridY, 24, 24, fot, GROUND_SOUND_NONE);
        case ForegroundObjectType_Stone_Middle:
            return new BlockingObject(gridX, gridY, 24, 24, fot, GROUND_SOUND_NONE);
        case ForegroundObjectType_Stone_Top:
            return new BlockingObject(gridX, gridY, 28, 24, fot, GROUND_SOUND_GRASS, 0, 0, 1, 0.95833333333333f);
        case ForegroundObjectType_Stone_Platform:
            return new BlockingObject(gridX, gridY, 28, 7, fot, GROUND_SOUND_GRASS, 0, 0, 1, 0.85714285714286f);
            
        case ForegroundObjectType_Floating_Platform:
            return new PlatformObject(gridX, gridY, 20, 11, fot, GROUND_SOUND_WOOD, 0, 0, 1, 0.875f); // Really should be a metallic sound
            
        case ForegroundObjectType_Stone_Square:
            return new BlockingObject(gridX, gridY, 24, 24, fot, GROUND_SOUND_NONE);
    }
    
    assert(false);
}

ForegroundObject::ForegroundObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType, float boundsX, float boundsY, float boundsWidth, float boundsHeight) : GridLockedPhysicalEntity(gridX, gridY, gridWidth, gridHeight, boundsX, boundsY, boundsWidth, boundsHeight), m_type(type), m_groundSoundType(groundSoundType), m_game(nullptr), m_color(1, 1, 1, 1)
{
    // Empty
}

bool ForegroundObject::isEntityLanding(PhysicalEntity* entity, float deltaTime)
{
    return isEntityLanding(entity, getMainBounds(), deltaTime);
}

bool ForegroundObject::isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime)
{
    return isEntityBlockedOnRight(entity, getMainBounds(), deltaTime);
}

bool ForegroundObject::isEntityBlockedOnLeft(PhysicalEntity* entity, float deltaTime)
{
	return isEntityBlockedOnLeft(entity, getMainBounds(), deltaTime);
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
		&& entity->getMainBounds().getRight() > bounds.getLeft())
	{
		float entityYDelta = fabsf(entityVelocityY * deltaTime);

		Rectangle tempBounds = Rectangle(bounds.getLeft(), bounds.getBottom(), bounds.getWidth(), bounds.getHeight());

		tempBounds.setHeight(tempBounds.getHeight() + entityYDelta);

		if (OverlapTester::doRectanglesOverlap(entity->getMainBounds(), tempBounds))
		{
			float jonLowerLeftY = entity->getMainBounds().getBottom();

			float itemTop = tempBounds.getTop();
			float padding = itemTop * .01f;
			padding += entityYDelta;
			float itemTopReq = itemTop - padding;

			if (jonLowerLeftY >= itemTopReq)
			{
				entity->placeOn(bounds.getTop());

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

bool ForegroundObject::isEntityBlockedOnRight(PhysicalEntity* entity, Rectangle& bounds, float deltaTime)
{
    if (OverlapTester::doRectanglesOverlap(entity->getMainBounds(), bounds))
    {
        float entityVelocityX = entity->getVelocity().getX();
        float entityBottom = entity->getMainBounds().getBottom();
        float entityRight = entity->getMainBounds().getRight();
        float entityXDelta = entityVelocityX * deltaTime;
        
        float itemTop = bounds.getTop();
        float itemTopReq = itemTop * 0.99f;
        
        float itemLeft = bounds.getLeft();
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

bool ForegroundObject::isEntityBlockedOnLeft(PhysicalEntity* entity, Rectangle& bounds, float deltaTime)
{
	if (OverlapTester::doRectanglesOverlap(entity->getMainBounds(), bounds))
	{
		float entityVelocityX = entity->getVelocity().getX();
		float entityBottom = entity->getMainBounds().getBottom();
		float entityLeft = entity->getMainBounds().getLeft();
		float entityXDelta = entityVelocityX * deltaTime;

		float itemTop = bounds.getTop();
		float itemTopReq = itemTop * 0.99f;

		float itemRight = bounds.getRight();
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

#pragma mark subclasses

bool PlatformObject::isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime)
{
    return false;
}

bool PlatformObject::isEntityBlockedOnLeft(PhysicalEntity* entity, float deltaTime)
{
	return false;
}

bool PlatformObject::canObjectBePlacedOn()
{
    return true;
}

bool DeadlyObject::isEntityLanding(PhysicalEntity* entity, float deltaTime)
{
    float entityY = entity->getPosition().getY();
    
    if (ForegroundObject::isEntityLanding(entity, deltaTime))
    {
        Jon *jon = nullptr;
        if (entity->getRTTI().derivesFrom(Jon::rtti))
        {
            jon = reinterpret_cast<Jon *>(entity);
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

bool DeadlyObject::isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime)
{
    if (ForegroundObject::isEntityBlockedOnRight(entity, deltaTime))
    {
        Jon *jon = nullptr;
        if (entity->getRTTI().derivesFrom(Jon::rtti))
        {
            jon = reinterpret_cast<Jon *>(entity);
            jon->kill();
        }
        
        return true;
    }
    
    return false;
}

bool DeadlyObject::isEntityBlockedOnRight(PhysicalEntity* entity, Rectangle& bounds, float deltaTime)
{
	if (OverlapTester::doRectanglesOverlap(entity->getMainBounds(), bounds))
	{
		float entityVelocityX = entity->getVelocity().getX();
		float entityBottom = entity->getMainBounds().getBottom();
		float entityRight = entity->getMainBounds().getRight();
		float entityXDelta = fabsf(entityVelocityX * deltaTime);

		float itemTop = bounds.getTop();
		float itemTopReq = itemTop * 0.99f;

		float itemLeft = bounds.getLeft();
		float padding = itemLeft * .01f;
		padding += entityXDelta;
		float itemLeftReq = itemLeft + padding;

		if (entityRight <= itemLeftReq && entityBottom < itemTopReq)
		{
			return true;
		}
	}

	return false;
}

bool DeadlyObject::isEntityBlockedOnLeft(PhysicalEntity* entity, float deltaTime)
{
	if (ForegroundObject::isEntityBlockedOnLeft(entity, deltaTime))
	{
        Jon *jon = nullptr;
        if (entity->getRTTI().derivesFrom(Jon::rtti))
        {
            jon = reinterpret_cast<Jon *>(entity);
			jon->kill();
		}

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
        Jon *jon = nullptr;
        if (entity->getRTTI().derivesFrom(Jon::rtti))
        {
            jon = reinterpret_cast<Jon *>(entity);
            jon->kill();
        }
    }
    
    return false;
}

bool RunningIntoDeathObject::isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime)
{
    if (ForegroundObject::isEntityBlockedOnRight(entity, deltaTime))
    {
        Jon *jon = nullptr;
        if (entity->getRTTI().derivesFrom(Jon::rtti))
        {
            jon = reinterpret_cast<Jon *>(entity);
            jon->kill();
        }
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
        
        Jon *jon = nullptr;
        if (entity->getRTTI().derivesFrom(Jon::rtti))
        {
            jon = reinterpret_cast<Jon *>(entity);
            jon->triggerBoost(m_fBoostVelocity);
        }
        
        m_isBoosting = true;
        m_fStateTime = 0;
    }
    
    return false;
}

bool JumpSpringLightFlush::isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime)
{
    return false;
}

bool JumpSpringLightFlush::isEntityBlockedOnLeft(PhysicalEntity* entity, float deltaTime)
{
	return false;
}

bool JumpSpringLightFlush::isEntityLanding(PhysicalEntity* entity, Rectangle& bounds, float deltaTime)
{
    float entityVelocityY = entity->getVelocity().getY();
    
    if (entityVelocityY <= 0
        && entity->getPosition().getX() > getMainBounds().getLeft()
        && (entity->getMainBounds().getBottom() + 0.01f) > getMainBounds().getBottom())
    {
        if (OverlapTester::doRectanglesOverlap(entity->getMainBounds(), bounds))
        {
            float itemTop = bounds.getTop();
            
			entity->placeOn(itemTop);
            
            Jon *jon = nullptr;
            if (entity->getRTTI().derivesFrom(Jon::rtti))
            {
                jon = reinterpret_cast<Jon *>(entity);
                jon->setGroundSoundType(getGroundSoundType());
            }
            
            return true;
        }
    }
    
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
    
    Rectangle& bounds = *getBounds().at(1);
    
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
    
    Jon *jon = nullptr;
    if (entity->getRTTI().derivesFrom(Jon::rtti))
    {
        jon = reinterpret_cast<Jon *>(entity);
        jon->getMainBounds().setAngle(jon->getAbilityState() == ABILITY_GLIDE ? 90 : 0);
        
        if (ForegroundObject::isEntityLanding(jon, deltaTime)
            || ForegroundObject::isEntityLanding(jon, *getBounds().at(1), deltaTime))
        {
            if (jon)
            {
                jon->kill();
            }
            
            ret = true;
        }
        
        jon->getMainBounds().setAngle(0);
    }
    
    return ret;
}

bool SpikeTower::isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime)
{
    bool ret = false;
    
    Jon *jon = nullptr;
    if (entity->getRTTI().derivesFrom(Jon::rtti))
    {
        jon = reinterpret_cast<Jon *>(entity);
        jon->getMainBounds().setAngle(jon->getAbilityState() == ABILITY_GLIDE ? 90 : 0);
    }
    
    if (ForegroundObject::isEntityBlockedOnRight(entity, deltaTime)
        || ForegroundObject::isEntityBlockedOnRight(entity, *getBounds().at(1), deltaTime))
    {
        if (jon)
        {
            jon->kill();
        }
        
        ret = true;
    }
    
    if (jon)
    {
        jon->getMainBounds().setAngle(0);
    }
    
    return ret;
}

void VerticalSaw::updateBounds()
{
    DeadlyObject::updateBounds();
    
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
            
            ASSETS->addSoundIdToPlayQueue(SOUND_SAW_GRIND);
        }
    }
    else if (m_isOnScreen)
    {
        m_isOnScreen = false;
        
        ASSETS->forceAddSoundIdToPlayQueue(STOP_SOUND_SAW_GRIND);
    }
}

void SpikedBallRollingLeft::update(float deltaTime)
{
    DeadlyObject::update(deltaTime);

	if (!m_isActivated)
	{
		Jon& jon = m_game->getJon();
		if (jon.getPosition().getX() > (getPosition().getX() - CAM_WIDTH * 2))
		{
			m_needsToPlaySound = true;

			m_isActivated = true;
		}
	}

	if (m_isActivated)
    {
        m_fAngle -= m_velocity->getX() * deltaTime * 15;
        
        if (m_velocity->getX() < (-VAMP_DEFAULT_MAX_SPEED * 2))
        {
            m_velocity->setX(-VAMP_DEFAULT_MAX_SPEED * 2);
        }

		if (!m_isStopped)
		{
			if (EntityUtils::isBlockedOnLeft(this, m_game->getEndBossForegroundObjects(), deltaTime))
			{
				stop();
			}
		}

		if (m_game->isEntityGrounded(this, deltaTime))
		{
			m_velocity->setY(0);
			m_acceleration->setY(0);

			if (!m_isStopped)
			{
				m_acceleration->setX(-VAMP_DEFAULT_ACCELERATION);

				if (m_needsToPlaySound)
				{
					ASSETS->forceAddSoundIdToPlayQueue(SOUND_SPIKED_BALL_ROLLING);

					m_needsToPlaySound = false;
				}
			}
		}
		else
		{
			if (!m_hasFallen)
			{
				if (EntityUtils::isBlockedOnLeft(this, m_game->getGrounds(), deltaTime))
				{
					m_velocity->setX(0);
					m_acceleration->setX(0);

					ASSETS->forceAddSoundIdToPlayQueue(STOP_SOUND_SPIKED_BALL_ROLLING);

					m_hasFallen = true;
					m_needsToPlaySound = true;
				}

				m_acceleration->setY(GAME_GRAVITY);
			}
		}
    }
}

void SpikedBallRollingLeft::updateBounds()
{
    DeadlyObject::updateBounds();
    
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
        }
    }
    else if (m_isOnScreen)
    {
        m_isOnScreen = false;
    }
}

void SpikedBallRollingLeft::stop()
{
	m_isStopped = true;

	m_velocity->setX(0);
	m_acceleration->setX(0);

	ASSETS->forceAddSoundIdToPlayQueue(STOP_SOUND_SPIKED_BALL_ROLLING);
}

void SpikedBallRollingRight::update(float deltaTime)
{
    DeadlyObject::update(deltaTime);
    
	if (!m_isActivated)
	{
		Jon& jon = m_game->getJon();
		if (jon.getMainBounds().getLeft() > getMainBounds().getLeft())
		{
			m_needsToPlaySound = true;
			m_isActivated = true;
		}
	}

    if (m_isActivated)
    {
		m_fAngle -= m_velocity->getX() * deltaTime * 15;
        
        if (m_velocity->getX() > (VAMP_DEFAULT_MAX_SPEED * 2))
        {
            m_velocity->setX((VAMP_DEFAULT_MAX_SPEED * 2));
        }

		if (!m_isStopped)
		{
			if (EntityUtils::isBlockedOnRight(this, m_game->getEndBossForegroundObjects(), deltaTime))
			{
				stop();
			}
		}

		if (m_game->isEntityGrounded(this, deltaTime))
		{
			m_velocity->setY(0);
			m_acceleration->setY(0);

			if (!m_isStopped)
			{
				m_acceleration->setX(VAMP_DEFAULT_ACCELERATION * 1.4f);

				if (m_needsToPlaySound)
				{
					ASSETS->forceAddSoundIdToPlayQueue(SOUND_SPIKED_BALL_ROLLING);

					m_needsToPlaySound = false;
				}
			}
		}
		else
		{
			if (!m_hasFallen)
			{
				if (EntityUtils::isBlockedOnRight(this, m_game->getGrounds(), deltaTime))
				{
					m_velocity->setX(0);
					m_acceleration->setX(0);

					ASSETS->forceAddSoundIdToPlayQueue(STOP_SOUND_SPIKED_BALL_ROLLING);

					m_hasFallen = true;
					m_needsToPlaySound = true;
				}

				m_acceleration->setY(GAME_GRAVITY);
			}
		}
    }
}

void SpikedBallRollingRight::updateBounds()
{
    DeadlyObject::updateBounds();
    
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
        }
    }
    else if (m_isOnScreen)
    {
        m_isOnScreen = false;
    }
}

void SpikedBallRollingRight::stop()
{
	m_isStopped = true;

	m_velocity->setX(0);
	m_acceleration->setX(0);

	ASSETS->forceAddSoundIdToPlayQueue(STOP_SOUND_SPIKED_BALL_ROLLING);
}

void SpikedBall::update(float deltaTime)
{
    if (m_isFalling)
    {
        DeadlyObject::update(deltaTime);
        
		EndBossSnake* snake = m_game->getEndBossSnakeP();

		if (snake)
		{
			if (!m_hasTriggeredSnakeHit
				&& OverlapTester::doRectanglesOverlap(snake->getMainBounds(), getMainBounds()))
			{
				snake->triggerHit();
				m_hasTriggeredSnakeHit = true;
			}
		}

		if (m_hasTriggeredSnakeHit)
		{
			m_acceleration->setY(0);
			m_velocity->setY(0);

			m_color.alpha -= deltaTime;
			if (m_color.alpha < 0)
			{
				m_color.alpha = 0;

				m_isRequestingDeletion = true;
			}
		}
		else
		{
			m_acceleration->setY(GAME_GRAVITY);
		}
    }
}

void SpikedBallChain::update(float deltaTime)
{
    if (m_isSnapping)
    {
        m_fStateTime += deltaTime;
        
        if (m_fStateTime > 0.30f)
        {
            if (!m_hasTriggeredSpikedBall)
            {
                if (m_spikedBall)
                {
                    m_spikedBall->fall();
                }
                
                m_hasTriggeredSpikedBall = true;
            }
            
            m_color.alpha -= deltaTime;
            if (m_color.alpha < 0)
            {
                m_color.alpha = 0;
                
                m_isRequestingDeletion = true;
            }
        }
    }
}

bool SpikedBallChain::isJonHittingHorizontally(Jon& jon, float deltaTime)
{
    Rectangle& bounds = jon.getMainBounds();
    
    if (!m_isSnapping
        && OverlapTester::doRectanglesOverlap(bounds, getMainBounds()))
    {
        m_isSnapping = true;
        m_fStateTime = 0;
        
        ASSETS->addSoundIdToPlayQueue(SOUND_CHAIN_SNAP);
        
        return true;
    }
    
    return false;
}

bool SpikedBallChain::isJonHittingFromBelow(Jon& jon, float deltaTime)
{
    Rectangle& bounds = jon.getMainBounds();
    
    if (!m_isSnapping
        && OverlapTester::doRectanglesOverlap(bounds, getMainBounds()))
    {
        m_isSnapping = true;
        m_fStateTime = 0;
        
        ASSETS->addSoundIdToPlayQueue(SOUND_CHAIN_SNAP);
        
        return true;
    }
    
    return false;
}

void SpikedBallChain::trigger()
{
	m_color.alpha = 0;
}

bool BlockingObject::isJonBlockedAbove(Jon &jon, float deltaTime)
{
    float entityVelocityY = jon.getVelocity().getY();
    
    if (entityVelocityY > 0 && OverlapTester::doRectanglesOverlap(jon.getMainBounds(), getMainBounds()))
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

RTTI_IMPL(ForegroundObject, GridLockedPhysicalEntity);
RTTI_IMPL(PlatformObject, ForegroundObject);
RTTI_IMPL(DeadlyObject, ForegroundObject);
RTTI_IMPL(LandingDeathObject, ForegroundObject);
RTTI_IMPL(RunningIntoDeathObject, ForegroundObject);
RTTI_IMPL(DeathFromAboveObject, ForegroundObject);
RTTI_IMPL(ProvideBoostObject, ForegroundObject);
RTTI_IMPL(EndSign, ForegroundObject);
RTTI_IMPL(JumpSpringLightFlush, ProvideBoostObject);
RTTI_IMPL(VerticalSaw, DeadlyObject);
RTTI_IMPL(GiantShakingTree, ForegroundObject);
RTTI_IMPL(ExtraForegroundObject, ForegroundObject);
RTTI_IMPL(SpikeTower, ExtraForegroundObject);
RTTI_IMPL(SpikedBallRollingLeft, DeadlyObject);
RTTI_IMPL(SpikedBallRollingRight, DeadlyObject);
RTTI_IMPL(SpikedBall, DeadlyObject);
RTTI_IMPL(SpikedBallChain, ForegroundObject);
RTTI_IMPL(BlockingObject, ForegroundObject);
