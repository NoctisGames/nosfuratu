//
//  ForegroundObject.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/9/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "pch.h"

#include "ForegroundObject.h"

#include "Game.h"
#include "Jon.h"
#include "NGRect.h"

#include "EntityUtils.h"
#include "Assets.h"
#include "NGAudioEngine.h"
#include "NGSTDUtil.h"
#include "GameConstants.h"
#include "EndBossSnake.h"

#include <math.h>

ForegroundObject* ForegroundObject::create(int gridX, int gridY, int type)
{
    ForegroundObjectType fot = (ForegroundObjectType)type;
    switch (fot)
    {
        case ForegroundObjectType_GrassPlatformLeft:
            return new PlatformObject(gridX, gridY, 4, 6, fot, GROUND_SOUND_ID_GRASS, 0, 0, 1, 0.83333333333333f);
        case ForegroundObjectType_GrassPlatformCenter:
            return new PlatformObject(gridX, gridY, 14, 6, fot, GROUND_SOUND_ID_GRASS, 0, 0, 1, 0.83333333333333f);
        case ForegroundObjectType_GrassPlatformRight:
            return new PlatformObject(gridX, gridY, 4, 6, fot, GROUND_SOUND_ID_GRASS, 0, 0, 1, 0.83333333333333f);
            
        case ForegroundObjectType_CavePlatformLeft:
            return new PlatformObject(gridX, gridY, 4, 6, fot, GROUND_SOUND_ID_CAVE, 0, 0, 1, 0.83333333333333f);
        case ForegroundObjectType_CavePlatformCenter:
            return new PlatformObject(gridX, gridY, 14, 6, fot, GROUND_SOUND_ID_CAVE, 0, 0, 1, 0.83333333333333f);
        case ForegroundObjectType_CavePlatformRight:
            return new PlatformObject(gridX, gridY, 4, 6, fot, GROUND_SOUND_ID_CAVE, 0, 0, 1, 0.83333333333333f);
            
        case ForegroundObjectType_RockLarge:
            return new ForegroundObject(gridX, gridY, 44, 32, fot, GROUND_SOUND_ID_CAVE, 0.03551136363636f, 0.0078125f, 0.77840909090909f, 0.96875f);
        case ForegroundObjectType_RockMedium:
            return new ForegroundObject(gridX, gridY, 24, 24, fot, GROUND_SOUND_ID_CAVE, 0.0625f, 0.015625f, 0.7734375f, 0.73958333333333f);
        case ForegroundObjectType_RockSmall:
        case ForegroundObjectType_RockSmallCracked:
            return new ForegroundObject(gridX, gridY, 24, 23, fot, GROUND_SOUND_ID_CAVE, 0.328125f, 0.20923913043478f, 0.328125f, 0.27717391304348f);
            
        case ForegroundObjectType_StumpBig:
            return new ForegroundObject(gridX, gridY, 16, 18, fot, GROUND_SOUND_ID_GRASS, 0.19140625f, 0.03125f, 0.5f, 0.94444444444444f);
        case ForegroundObjectType_StumpSmall:
            return new ForegroundObject(gridX, gridY, 20, 17, fot, GROUND_SOUND_ID_GRASS, 0.215625f, 0.07352941176471f, 0.375f, 0.73897058823529f);
            
        case ForegroundObjectType_EndSign:
            return new EndSign(gridX, gridY, 7, 10, fot);
            
        case ForegroundObjectType_ThornsLeft:
            return new DeadlyObject(gridX, gridY, 5, 7, fot, GROUND_SOUND_ID_NONE, 0, 0, 1, 0.8f);
        case ForegroundObjectType_ThornsCenterSmall:
            return new DeadlyObject(gridX, gridY, 11, 7, fot, GROUND_SOUND_ID_NONE, 0, 0, 1, 0.8f);
        case ForegroundObjectType_ThornsCenterBig:
            return new DeadlyObject(gridX, gridY, 22, 7, fot, GROUND_SOUND_ID_NONE, 0, 0, 1, 0.8f);
        case ForegroundObjectType_ThornsRight:
            return new DeadlyObject(gridX, gridY, 5, 7, fot, GROUND_SOUND_ID_NONE, 0, 0, 1, 0.8f);
            
        case ForegroundObjectType_LogVerticalTall:
            return new ForegroundObject(gridX, gridY, 8, 11, fot, GROUND_SOUND_ID_GRASS, 0.1484375f, 0.02840909090909f, 0.5f, 0.92613636363636f);
        case ForegroundObjectType_LogVerticalShort:
            return new ForegroundObject(gridX, gridY, 8, 6, fot, GROUND_SOUND_ID_GRASS, 0.1462f, 0.05208333333333f, 0.4453125f, 0.875f);
            
        case ForegroundObjectType_JumpSpringLightFlushNew:
            return new JumpSpringLightFlush(gridX, gridY, 11, 11, fot, GROUND_SOUND_ID_NONE, 0, 0.363636363636364f, 1, 0.1f, 18.0f);
        case ForegroundObjectType_JumpSpringLightFlush:
            return new JumpSpringLightFlush(gridX, gridY, 10, 7, fot, GROUND_SOUND_ID_NONE, 0, 0.033f, 1, 0.1f, 18.0f);
        case ForegroundObjectType_JumpSpringLight:
            return new ProvideBoostObject(gridX, gridY, 6, 5, fot, GROUND_SOUND_ID_NONE, 0, 0, 1, 0.525f, 18.0f);
        case ForegroundObjectType_JumpSpringMedium:
            return new ProvideBoostObject(gridX, gridY, 17, 9, fot, GROUND_SOUND_ID_NONE, 0, 0.20138888888889f, 0.6f, 0.33333333333333f, 24.0f);
        case ForegroundObjectType_JumpSpringHeavy:
            return new ProvideBoostObject(gridX, gridY, 17, 14, fot, GROUND_SOUND_ID_NONE, 0, 0, 0.6f, 0.52678571428571f, 32.0f);
            
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
            return new DeadlyObject(gridX, gridY, 14, 14, fot, GROUND_SOUND_ID_NONE, 0.18f, 0.18f, 0.64f, 0.64f);
            
        case ForegroundObjectType_VerticalSaw:
            return new VerticalSaw(gridX, 95, 15, 33, fot, GROUND_SOUND_ID_NONE, 0.3f, 0.33333333333333f, 0.3f, 0.66666666666667f);
            
        case ForegroundObjectType_GiantTree:
            return new ForegroundObject(gridX, gridY, 68, 65, fot, GROUND_SOUND_ID_NONE, 0.47265625f, 0.009765625f, 0.15625f, 0.990234375f);
        case ForegroundObjectType_GiantShakingTree:
            return new GiantShakingTree(gridX, gridY, 68, 65, fot, GROUND_SOUND_ID_NONE, 0.47265625f, 0.009765625f, 0.15625f, 0.990234375f);
        case ForegroundObjectType_GiantPerchTree:
            return new ForegroundObject(gridX, gridY, 68, 65, fot, GROUND_SOUND_ID_NONE, 0, 0.009765625f, 1, 0.990234375f);
            
        case ForegroundObjectType_SpikeTower:
            return new SpikeTower(gridX, 95, 32, 56, fot, ForegroundObjectType_SpikeTowerBg, GROUND_SOUND_ID_NONE, 0, 0, 1, 0.54017857142857f);
        case ForegroundObjectType_SpikeTowerBg:
            return new ForegroundObject(gridX, 95, 32, 56, fot);
            
        case ForegroundObjectType_SpikedBallRollingLeft:
            return new SpikedBallRollingLeft(gridX, gridY, 32, 30, fot, GROUND_SOUND_ID_NONE, 0.1f, 0.1f, 0.8f, 0.8f);
        case ForegroundObjectType_SpikedBallRollingRight:
            return new SpikedBallRollingRight(gridX, gridY, 32, 30, fot, GROUND_SOUND_ID_NONE, 0.1f, 0.1f, 0.8f, 0.8f);
            
        case ForegroundObjectType_SpikedBallChain:
            return new SpikedBallChain(gridX, gridY, 101, 82, fot, GROUND_SOUND_ID_NONE, 0.72f, 0, 0.14f, 0.25f);
        case ForegroundObjectType_SpikedBall:
            return new SpikedBall(gridX, gridY, 32, 30, fot, GROUND_SOUND_ID_NONE, 0, 0.4f, 1, 0.8f);
            
        case ForegroundObjectType_MetalGrassPlatform:
            return new PlatformObject(gridX, gridY, 16, 6, fot, GROUND_SOUND_ID_GRASS, 0, 0, 1, 0.89583333333333f);
        case ForegroundObjectType_MetalGrassPlatformLeft:
            return new PlatformObject(gridX, gridY, 4, 6, fot, GROUND_SOUND_ID_GRASS, 0, 0, 1, 0.89583333333333f);
        case ForegroundObjectType_MetalGrassPlatformCenter:
            return new PlatformObject(gridX, gridY, 16, 6, fot, GROUND_SOUND_ID_GRASS, 0, 0, 1, 0.89583333333333f);
        case ForegroundObjectType_MetalGrassPlatformRight:
            return new PlatformObject(gridX, gridY, 4, 6, fot, GROUND_SOUND_ID_GRASS, 0, 0, 1, 0.89583333333333f);
            
        case ForegroundObjectType_WoodPlatform:
            return new PlatformObject(gridX, gridY, 16, 4, fot, GROUND_SOUND_ID_WOOD, 0, 0, 1, 0.875f);
        case ForegroundObjectType_WoodBoxTop:
            return new ForegroundObject(gridX, gridY, 16, 16, fot, GROUND_SOUND_ID_WOOD, 0, 0, 1, 0.96875f);
        case ForegroundObjectType_WoodBox:
            return new ForegroundObject(gridX, gridY, 16, 16, fot, GROUND_SOUND_ID_WOOD);
            
        case ForegroundObjectType_GreenThornsLeft:
            return new DeadlyObject(gridX, gridY, 8, 8, fot, GROUND_SOUND_ID_NONE, 0, 0, 1, 0.8f);
        case ForegroundObjectType_GreenThornsCenterSmall:
            return new DeadlyObject(gridX, gridY, 12, 8, fot, GROUND_SOUND_ID_NONE, 0, 0, 1, 0.8f);
        case ForegroundObjectType_GreenThornsCenterBig:
            return new DeadlyObject(gridX, gridY, 24, 8, fot, GROUND_SOUND_ID_NONE, 0, 0, 1, 0.8f);
        case ForegroundObjectType_GreenThornsRight:
            return new DeadlyObject(gridX, gridY, 8, 8, fot, GROUND_SOUND_ID_NONE, 0, 0, 1, 0.8f);
            
        case ForegroundObjectType_Logs:
            return new ForegroundObject(gridX, gridY, 37, 10, fot, GROUND_SOUND_ID_GRASS, 0, 0, 1, 0.9375f);
            
        case ForegroundObjectType_Stone_Bottom:
            return new BlockingObject(gridX, gridY, 24, 24, fot, GROUND_SOUND_ID_NONE);
        case ForegroundObjectType_Stone_Middle:
            return new BlockingObject(gridX, gridY, 24, 24, fot, GROUND_SOUND_ID_NONE);
        case ForegroundObjectType_Stone_Top:
            return new BlockingObject(gridX, gridY, 28, 24, fot, GROUND_SOUND_ID_GRASS, 0, 0, 1, 0.95833333333333f);
        case ForegroundObjectType_Stone_Platform:
            return new BlockingObject(gridX, gridY, 28, 7, fot, GROUND_SOUND_ID_GRASS, 0, 0, 1, 0.85714285714286f);
            
        case ForegroundObjectType_Floating_Platform:
            return new FloatingPlatformObject(gridX, gridY, 20, 11, fot, GROUND_SOUND_ID_WOOD, 0, 0, 1, 0.875f); // Really should be a metallic sound
            
        case ForegroundObjectType_Stone_Square:
            return new BlockingObject(gridX, gridY, 24, 24, fot, GROUND_SOUND_ID_NONE);
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

int ForegroundObject::getEntityLandingPriority()
{
    return 0;
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

Color ForegroundObject::getColor()
{
    return m_color;
}

#pragma mark protected

bool ForegroundObject::isEntityLanding(PhysicalEntity* entity, NGRect& bounds, float deltaTime)
{
	float entityVelocityY = entity->getVelocity().getY();

	if (entityVelocityY <= 0
		&& entity->getMainBounds().getRight() > bounds.getLeft())
	{
		float entityYDelta = fabsf(entityVelocityY * deltaTime);

		NGRect tempBounds = NGRect(bounds.getLeft(), bounds.getBottom(), bounds.getWidth(), bounds.getHeight());

		tempBounds.setHeight(tempBounds.getHeight() + entityYDelta);

		if (OverlapTester::doNGRectsOverlap(entity->getMainBounds(), tempBounds))
		{
			float jonLowerLeftY = entity->getMainBounds().getBottom();

			float itemTop = tempBounds.getTop();
			float padding = GRID_CELL_SIZE / 2;
			padding += entityYDelta;
			float itemTopReq = itemTop - padding;

			if (jonLowerLeftY >= itemTopReq)
			{
                EntityUtils::placeOn(entity, bounds.getTop());

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

bool ForegroundObject::isEntityBlockedOnRight(PhysicalEntity* entity, NGRect& bounds, float deltaTime)
{
    if (OverlapTester::doNGRectsOverlap(entity->getMainBounds(), bounds))
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

bool ForegroundObject::isEntityBlockedOnLeft(PhysicalEntity* entity, NGRect& bounds, float deltaTime)
{
	if (OverlapTester::doNGRectsOverlap(entity->getMainBounds(), bounds))
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

PlatformObject::PlatformObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType, float boundsX, float boundsY, float boundsWidth, float boundsHeight) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType, boundsX, boundsY, boundsWidth, boundsHeight)
{
    // Empty
}

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

bool PlatformObject::shouldJonGrabLedge(PhysicalEntity* entity, float deltaTime)
{
    if (ForegroundObject::isEntityBlockedOnRight(entity, deltaTime))
    {
        return true;
    }
    
    return false;
}

FloatingPlatformObject::FloatingPlatformObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType, float boundsX, float boundsY, float boundsWidth, float boundsHeight) : PlatformObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType, boundsX, boundsY, boundsWidth, boundsHeight), m_fOriginalY(0), m_isIdle(true), m_isWeighted(false)
{
    float x = m_position.getX();
    float y = m_position.getY() - m_fHeight / 2 + 0.1f;
    m_idlePoof = new PhysicalEntity(x, y - 0.31640625f / 2, 0.4921875f, 0.31640625f);
    m_addedWeightPoof = new PhysicalEntity(x, y - 1.51171875f / 2, 1.40625f, 1.51171875f);
    
    onMoved();
}

FloatingPlatformObject::~FloatingPlatformObject()
{
    delete m_idlePoof;
    delete m_addedWeightPoof;
}

void FloatingPlatformObject::update(float deltaTime)
{
    m_position.add(m_velocity.getX() * deltaTime, m_velocity.getY() * deltaTime);
    
    float x = m_position.getX();
    float y = m_position.getY() - m_fHeight / 2 + 0.1f;
    m_idlePoof->getPosition().set(x, y - 0.31640625f / 2);
    m_addedWeightPoof->getPosition().set(x, y - 1.51171875f / 2);
    
    m_idlePoof->update(deltaTime);
    m_addedWeightPoof->update(deltaTime);
    
    if (m_isIdle)
    {
        if (m_position.getY() > (m_fOriginalY + 0.1f))
        {
            m_velocity.setY(-0.2f);
        }
        else if (m_position.getY() < (m_fOriginalY - 0.1f))
        {
            m_velocity.setY(0.2f);
        }
    }
}

bool FloatingPlatformObject::isEntityLanding(PhysicalEntity* entity, float deltaTime)
{
    if (PlatformObject::isEntityLanding(entity, deltaTime))
    {
        m_isIdle = false;
        m_isWeighted = true;
        
        m_position.setY(m_fOriginalY - 0.2f);
        m_velocity.setY(0);
        
        return true;
    }
    
    m_isIdle = true;
    m_isWeighted = false;
    
    return false;
}

void FloatingPlatformObject::onMoved()
{
    m_fOriginalY = m_position.getY();
    
    // One time
    getMainBounds().setWidth(getWidth());
    getMainBounds().setHeight(getHeight());
    
    PhysicalEntity::updateBounds();
    
    getMainBounds().getLowerLeft().add(getWidth() * m_fBoundsX, getHeight() * m_fBoundsY);
    getMainBounds().setWidth(getWidth() * m_fBoundsWidth);
    getMainBounds().setHeight(getHeight() * m_fBoundsHeight);
    
    m_velocity.setY(-0.25f);
}

PhysicalEntity& FloatingPlatformObject::getIdlePoof()
{
    return *m_idlePoof;
}

PhysicalEntity& FloatingPlatformObject::getAddedWeightPoof()
{
    return *m_addedWeightPoof;
}

bool FloatingPlatformObject::isIdle()
{
    return m_isIdle;
}

bool FloatingPlatformObject::isWeighted()
{
    return m_isWeighted;
}

DeadlyObject::DeadlyObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType, float boundsX, float boundsY, float boundsWidth, float boundsHeight) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType, boundsX, boundsY, boundsWidth, boundsHeight)
{
    // Empty
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

int DeadlyObject::getEntityLandingPriority()
{
    return 1;
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

bool DeadlyObject::isEntityBlockedOnRight(PhysicalEntity* entity, NGRect& bounds, float deltaTime)
{
	if (OverlapTester::doNGRectsOverlap(entity->getMainBounds(), bounds))
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
    
    if (entityVelocityY > 0 && OverlapTester::doNGRectsOverlap(jon.getMainBounds(), getMainBounds()))
    {
        jon.getPosition().sub(0, jon.getVelocity().getY() * deltaTime);
        jon.updateBounds();
        jon.kill();
    }
    
    return false;
}

LandingDeathObject::LandingDeathObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, float boundsX, float boundsY, float boundsWidth, float boundsHeight) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, GROUND_SOUND_ID_NONE, boundsX, boundsY, boundsWidth, boundsHeight)
{
    // Empty
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

int LandingDeathObject::getEntityLandingPriority()
{
    return 1;
}

RunningIntoDeathObject::RunningIntoDeathObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type)
{
    // Empty
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

DeathFromAboveObject::DeathFromAboveObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, float boundsX, float boundsY, float boundsWidth, float boundsHeight) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, GROUND_SOUND_ID_NONE, boundsX, boundsY, boundsWidth, boundsHeight)
{
    // Empty
}

bool DeathFromAboveObject::isJonBlockedAbove(Jon& jon, float deltaTime)
{
    float entityVelocityY = jon.getVelocity().getY();
    
    if (entityVelocityY > 0 && OverlapTester::doNGRectsOverlap(jon.getMainBounds(), getMainBounds()))
    {
        jon.getPosition().sub(0, jon.getVelocity().getY() * deltaTime);
        jon.updateBounds();
        jon.kill();
    }
    
    return false;
}

ProvideBoostObject::ProvideBoostObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType, float boundsX, float boundsY, float boundsWidth, float boundsHeight, float boostVelocity) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType, boundsX, boundsY, boundsWidth, boundsHeight), m_fBoostVelocity(boostVelocity), m_isBoosting(false)
{
    // Empty
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

int ProvideBoostObject::getEntityLandingPriority()
{
    return 1;
}

ExtraForegroundObject* ExtraForegroundObject::create(int gridX, int gridY, int type)
{
    return reinterpret_cast<ExtraForegroundObject *>(ForegroundObject::create(gridX, gridY, type));
}

ExtraForegroundObject::ExtraForegroundObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, ForegroundObjectType shadowType, GroundSoundType groundSoundType, float boundsX, float boundsY, float boundsWidth, float boundsHeight) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType, boundsX, boundsY, boundsWidth, boundsHeight), m_shadow(nullptr)
{
    m_shadow = ForegroundObject::create(gridX, gridY, shadowType);
}

ExtraForegroundObject::~ExtraForegroundObject()
{
    delete m_shadow;
}

void ExtraForegroundObject::update(float deltaTime)
{
    ForegroundObject::update(deltaTime);
    
    m_shadow->update(deltaTime);
    m_shadow->getPosition().set(getPosition());
    m_shadow->updateBounds();
}

ForegroundObject& ExtraForegroundObject::getShadow()
{
    return *m_shadow;
}

EndSign::EndSign(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type)
{
    // Empty
}

bool EndSign::isEntityLanding(PhysicalEntity* entity, float deltaTime)
{
    return false;
}

bool EndSign::isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime)
{
    return false;
}

bool EndSign::isEntityBlockedOnLeft(PhysicalEntity* entity, float deltaTime)
{
    return false;
}

JumpSpringLightFlush::JumpSpringLightFlush(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType, float boundsX, float boundsY, float boundsWidth, float boundsHeight, float boostVelocity) : ProvideBoostObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType, boundsX, boundsY, boundsWidth, boundsHeight, boostVelocity)
{
    // Empty
}

bool JumpSpringLightFlush::isEntityLanding(PhysicalEntity* entity, NGRect& bounds, float deltaTime)
{
    float entityVelocityY = entity->getVelocity().getY();
    
    if (entityVelocityY <= 0
        && entity->getPosition().getX() > getMainBounds().getLeft()
        && (entity->getMainBounds().getBottom() + 0.01f) > getMainBounds().getBottom())
    {
        if (OverlapTester::doNGRectsOverlap(entity->getMainBounds(), bounds))
        {
            float itemTop = bounds.getTop();
            
            EntityUtils::placeOn(entity, itemTop);
            
            Jon *jon = nullptr;
            if (entity->getRTTI().derivesFrom(Jon::rtti))
            {
                jon = reinterpret_cast<Jon *>(entity);
                jon->setGroundSoundType(getGroundSoundType());
                jon->setLastSpringBouncedOn(this);
            }
            
            return true;
        }
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

VerticalSaw::VerticalSaw(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType, float boundsX, float boundsY, float boundsWidth, float boundsHeight) : DeadlyObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType, boundsX, boundsY, boundsWidth, boundsHeight), m_isOnScreen(false)
{
    // Empty
}

void VerticalSaw::updateBounds()
{
    DeadlyObject::updateBounds();
    
    NGRect& camBounds = *m_game->getCameraBounds();
    
    if (camBounds.getWidth() > CAM_WIDTH)
    {
        return;
    }
    
    if (OverlapTester::doNGRectsOverlap(camBounds, getMainBounds()))
    {
        if (!m_isOnScreen)
        {
            m_isOnScreen = true;
            
            NG_AUDIO_ENGINE->playSound(SOUND_ID_SAW_GRIND, true);
        }
    }
    else if (m_isOnScreen)
    {
        m_isOnScreen = false;
        
        NG_AUDIO_ENGINE->stopSound(SOUND_ID_SAW_GRIND);
    }
}

GiantShakingTree::GiantShakingTree(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType, float boundsX, float boundsY, float boundsWidth, float boundsHeight) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType, boundsX, boundsY, boundsWidth, boundsHeight), m_isShaking(false)
{
    // Empty
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

SpikeTower::SpikeTower(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, ForegroundObjectType shadowType, GroundSoundType groundSoundType, float boundsX, float boundsY, float boundsWidth, float boundsHeight) : ExtraForegroundObject(gridX, gridY, gridWidth, gridHeight, type, shadowType, groundSoundType, boundsX, boundsY, boundsWidth, boundsHeight)
{
    m_bounds.push_back(new NGRect(0, 0, 1, 1));
    
    updateBounds();
}

void SpikeTower::updateBounds()
{
    ForegroundObject::updateBounds();
    
    NGRect* bounds = getBounds().at(1);
    
    bounds->setWidth(getWidth());
    bounds->setHeight(getHeight());
    
    Vector2D &lowerLeft = bounds->getLowerLeft();
    lowerLeft.set(m_position.getX() - bounds->getWidth() / 2, m_position.getY() - bounds->getHeight() / 2);
    
    bounds->getLowerLeft().add(getWidth() * 0.0f, getHeight() * 0.79910714285714f);
    bounds->setWidth(getWidth() * 1.0f);
    bounds->setHeight(getHeight() * 0.20089285714286f);
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

SpikedBallRollingLeft::SpikedBallRollingLeft(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType, float boundsX, float boundsY, float boundsWidth, float boundsHeight) : DeadlyObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType, boundsX, boundsY, boundsWidth, boundsHeight),
m_isOnScreen(false),
m_isStopped(false),
m_needsToPlaySound(false),
m_isActivated(false),
m_hasFallen(false)
{
    // Empty
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
        m_fAngle -= m_velocity.getX() * deltaTime * 15;
        
        if (m_velocity.getX() < (-VAMP_DEFAULT_MAX_SPEED * 2))
        {
            m_velocity.setX(-VAMP_DEFAULT_MAX_SPEED * 2);
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
			m_velocity.setY(0);
			m_acceleration.setY(0);

			if (!m_isStopped)
			{
				m_acceleration.setX(-VAMP_DEFAULT_ACCELERATION);

				if (m_needsToPlaySound)
				{
					NG_AUDIO_ENGINE->playSound(SOUND_ID_SPIKED_BALL_ROLLING, true);

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
					m_velocity.setX(0);
					m_acceleration.setX(0);

					NG_AUDIO_ENGINE->stopSound(SOUND_ID_SPIKED_BALL_ROLLING);

					m_hasFallen = true;
					m_needsToPlaySound = true;
				}

				m_acceleration.setY(GAME_GRAVITY);
			}
		}
    }
}

void SpikedBallRollingLeft::updateBounds()
{
    DeadlyObject::updateBounds();
    
    NGRect& camBounds = *m_game->getCameraBounds();
    
    if (camBounds.getWidth() > CAM_WIDTH)
    {
        return;
    }
    
    if (OverlapTester::doNGRectsOverlap(camBounds, getMainBounds()))
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

bool SpikedBallRollingLeft::isEntityLanding(PhysicalEntity* entity, float deltaTime)
{
    return false;
}

void SpikedBallRollingLeft::stop()
{
	m_isStopped = true;

	m_velocity.setX(0);
	m_acceleration.setX(0);

	NG_AUDIO_ENGINE->stopSound(SOUND_ID_SPIKED_BALL_ROLLING);
}

SpikedBallRollingRight::SpikedBallRollingRight(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType, float boundsX, float boundsY, float boundsWidth, float boundsHeight) : DeadlyObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType, boundsX, boundsY, boundsWidth, boundsHeight),
m_isOnScreen(false),
m_isStopped(false),
m_needsToPlaySound(false),
m_isActivated(false),
m_hasFallen(false)
{
    // Empty
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
		m_fAngle -= m_velocity.getX() * deltaTime * 15;
        
        if (m_velocity.getX() > (VAMP_DEFAULT_MAX_SPEED * 2))
        {
            m_velocity.setX((VAMP_DEFAULT_MAX_SPEED * 2));
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
			m_velocity.setY(0);
			m_acceleration.setY(0);

			if (!m_isStopped)
			{
				m_acceleration.setX(VAMP_DEFAULT_ACCELERATION * 1.4f);

				if (m_needsToPlaySound)
				{
					NG_AUDIO_ENGINE->playSound(SOUND_ID_SPIKED_BALL_ROLLING, true);

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
					m_velocity.setX(0);
					m_acceleration.setX(0);

					NG_AUDIO_ENGINE->stopSound(SOUND_ID_SPIKED_BALL_ROLLING);

					m_hasFallen = true;
					m_needsToPlaySound = true;
				}

				m_acceleration.setY(GAME_GRAVITY);
			}
		}
    }
}

void SpikedBallRollingRight::updateBounds()
{
    DeadlyObject::updateBounds();
    
    NGRect& camBounds = *m_game->getCameraBounds();
    
    if (camBounds.getWidth() > CAM_WIDTH)
    {
        return;
    }
    
    if (OverlapTester::doNGRectsOverlap(camBounds, getMainBounds()))
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

bool SpikedBallRollingRight::isEntityLanding(PhysicalEntity* entity, float deltaTime)
{
    return false;
}

void SpikedBallRollingRight::stop()
{
	m_isStopped = true;

	m_velocity.setX(0);
	m_acceleration.setX(0);

	NG_AUDIO_ENGINE->stopSound(SOUND_ID_SPIKED_BALL_ROLLING);
}

SpikedBall::SpikedBall(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType, float boundsX, float boundsY, float boundsWidth, float boundsHeight) : DeadlyObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType, boundsX, boundsY, boundsWidth, boundsHeight), m_isFalling(false), m_hasTriggeredSnakeHit(false)
{
    // Empty
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
				&& OverlapTester::doNGRectsOverlap(snake->getMainBounds(), getMainBounds()))
			{
				snake->triggerHit();
				m_hasTriggeredSnakeHit = true;
			}
		}

		if (m_hasTriggeredSnakeHit)
		{
			m_acceleration.setY(0);
			m_velocity.setY(0);

			m_color.alpha -= deltaTime;
			if (m_color.alpha < 0)
			{
				m_color.alpha = 0;

				m_isRequestingDeletion = true;
			}
		}
		else
		{
			m_acceleration.setY(GAME_GRAVITY);
		}
    }
}

void SpikedBall::fall()
{
    m_isFalling = true;
    m_velocity.setY(GAME_GRAVITY);
}

SpikedBallChain::SpikedBallChain(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType, float boundsX, float boundsY, float boundsWidth, float boundsHeight) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType, boundsX, boundsY, boundsWidth, boundsHeight), m_spikedBall(nullptr), m_isSnapping(false), m_hasTriggeredSpikedBall(false)
{
    // Empty
}

bool SpikedBallChain::isEntityLanding(PhysicalEntity* entity, float deltaTime)
{
    return false;
}

bool SpikedBallChain::isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime)
{
    return false;
}

void SpikedBallChain::setSpikedBall(SpikedBall* spikedBall)
{
    m_spikedBall = spikedBall;
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
    NGRect& bounds = jon.getMainBounds();
    
    if (!m_isSnapping
        && OverlapTester::doNGRectsOverlap(bounds, getMainBounds()))
    {
        m_isSnapping = true;
        m_fStateTime = 0;
        
        NG_AUDIO_ENGINE->playSound(SOUND_ID_CHAIN_SNAP);
        
        return true;
    }
    
    return false;
}

bool SpikedBallChain::isJonHittingFromBelow(Jon& jon, float deltaTime)
{
    NGRect& bounds = jon.getMainBounds();
    
    if (!m_isSnapping
        && OverlapTester::doNGRectsOverlap(bounds, getMainBounds()))
    {
        m_isSnapping = true;
        m_fStateTime = 0;
        
        NG_AUDIO_ENGINE->playSound(SOUND_ID_CHAIN_SNAP);
        
        return true;
    }
    
    return false;
}

void SpikedBallChain::trigger()
{
	m_color.alpha = 0;
}

BlockingObject::BlockingObject(int gridX, int gridY, int gridWidth, int gridHeight, ForegroundObjectType type, GroundSoundType groundSoundType, float boundsX, float boundsY, float boundsWidth, float boundsHeight) : ForegroundObject(gridX, gridY, gridWidth, gridHeight, type, groundSoundType, boundsX, boundsY, boundsWidth, boundsHeight)
{
    switch (type)
    {
        case ForegroundObjectType_Stone_Bottom:
        {
            static float sz = 384.0f;
            
            m_bounds.push_back(new NGRect(0, 0, 1, 1));
            m_bounds.push_back(new NGRect(0, 0, 1, 1));
            m_bounds.push_back(new NGRect(0, 0, 1, 1));
            
            m_normalizedBounds.push_back(new NGRect(20 / sz, 0, 348 / sz, 157 / sz));
            m_normalizedBounds.push_back(new NGRect(40 / sz, 170 / sz, 304 / sz, 58 / sz));
            m_normalizedBounds.push_back(new NGRect(144 / sz, 230 / sz, 116 / sz, 138 / sz));
        }
            break;
        case ForegroundObjectType_Stone_Middle:
        {
            static float sz = 384.0f;
            
            m_bounds.push_back(new NGRect(0, 0, 1, 1));
            m_bounds.push_back(new NGRect(0, 0, 1, 1));
            m_bounds.push_back(new NGRect(0, 0, 1, 1));
            m_bounds.push_back(new NGRect(0, 0, 1, 1));
            m_bounds.push_back(new NGRect(0, 0, 1, 1));
            
            m_normalizedBounds.push_back(new NGRect(187 / sz, 12 / sz, 81 / sz, 78 / sz));
            m_normalizedBounds.push_back(new NGRect(77 / sz, 97 / sz, 200 / sz, 56 / sz));
            m_normalizedBounds.push_back(new NGRect(18 / sz, 163 / sz, 317 / sz, 50 / sz));
            m_normalizedBounds.push_back(new NGRect(66 / sz, 217 / sz, 249 / sz, 63 / sz));
            m_normalizedBounds.push_back(new NGRect(122 / sz, 282 / sz, 114 / sz, 73 / sz));
        }
            break;
        case ForegroundObjectType_Stone_Top:
        {
            static float szw = 448.0f;
            static float szh = 384.0f;
            
            m_bounds.push_back(new NGRect(0, 0, 1, 1));
            m_bounds.push_back(new NGRect(0, 0, 1, 1));
            m_bounds.push_back(new NGRect(0, 0, 1, 1));
            m_bounds.push_back(new NGRect(0, 0, 1, 1));
            
            m_normalizedBounds.push_back(new NGRect(8 / szw, 329 / szh, 432 / szw, 38 / szh));
            m_normalizedBounds.push_back(new NGRect(44 / szw, 255 / szh, 360 / szw, 69 / szh));
            m_normalizedBounds.push_back(new NGRect(118 / szw, 118 / szh, 243 / szw, 135 / szh));
            m_normalizedBounds.push_back(new NGRect(158 / szw, 32 / szh, 152 / szw, 84 / szh));
        }
            break;
        case ForegroundObjectType_Stone_Platform:
        {
            static float szw = 448.0f;
            static float szh = 112.0f;
            
            m_bounds.push_back(new NGRect(0, 0, 1, 1));
            m_bounds.push_back(new NGRect(0, 0, 1, 1));
            
            m_normalizedBounds.push_back(new NGRect(8 / szw, 57 / szh, 432 / szw, 38 / szh));
            m_normalizedBounds.push_back(new NGRect(41 / szw, 20 / szh, 358 / szw, 38 / szh));
        }
            break;
        case ForegroundObjectType_Stone_Square:
        {
            static float sz = 384.0f;
            
            m_bounds.push_back(new NGRect(0, 0, 1, 1));
            m_normalizedBounds.push_back(new NGRect(8 / sz, 8 / sz, 368 / sz, 368 / sz));
        }
            break;
        default:
            assert(false);
    }
}

BlockingObject::~BlockingObject()
{
    NGSTDUtil::cleanUpVectorOfPointers(m_normalizedBounds);
}

void BlockingObject::updateBounds()
{
    ForegroundObject::updateBounds();
    
    unsigned long len = getBounds().size();
    for (int i = 1; i < len; ++i)
    {
        NGRect* bounds = getBounds().at(i);
        Vector2D &lowerLeft = bounds->getLowerLeft();
        lowerLeft.set(getMainBounds().getLowerLeft());
        
        NGRect* normal = m_normalizedBounds.at(i - 1);
        lowerLeft.add(getWidth() * normal->getLeft(), getHeight() * normal->getBottom());
        bounds->setWidth(getWidth() * normal->getWidth());
        bounds->setHeight(getHeight() * normal->getHeight());
    }
}

bool BlockingObject::isEntityLanding(PhysicalEntity* entity, float deltaTime)
{
    unsigned long len = getBounds().size();
    for (int i = 1; i < len; ++i)
    {
        NGRect* bounds = getBounds().at(i);
        if (ForegroundObject::isEntityLanding(entity, *bounds, deltaTime))
        {
            return true;
        }
    }
    
    return false;
}

bool BlockingObject::isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime)
{
    unsigned long len = getBounds().size();
    for (int i = 1; i < len; ++i)
    {
        NGRect* bounds = getBounds().at(i);
        if (ForegroundObject::isEntityBlockedOnRight(entity, *bounds, deltaTime))
        {
            return true;
        }
    }
    
    return false;
}

bool BlockingObject::isEntityBlockedOnLeft(PhysicalEntity* entity, float deltaTime)
{
    unsigned long len = getBounds().size();
    for (int i = 1; i < len; ++i)
    {
        NGRect* bounds = getBounds().at(i);
        if (ForegroundObject::isEntityBlockedOnLeft(entity, *bounds, deltaTime))
        {
            return true;
        }
    }
    
    return false;
}

bool BlockingObject::isJonBlockedAbove(Jon &jon, float deltaTime)
{
    float entityVelocityY = jon.getVelocity().getY();
    
    if (entityVelocityY > 0)
    {
        unsigned long len = getBounds().size();
        for (int i = 1; i < len; ++i)
        {
            NGRect* bounds = getBounds().at(i);
            if (OverlapTester::doNGRectsOverlap(jon.getMainBounds(), *bounds))
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
        }
    }
    
    return false;
}

RTTI_IMPL(ForegroundObject, GridLockedPhysicalEntity);
RTTI_IMPL(PlatformObject, ForegroundObject);
RTTI_IMPL(FloatingPlatformObject, PlatformObject);
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
