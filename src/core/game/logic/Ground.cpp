//
//  Ground.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/5/15.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "pch.h"

#include "Ground.h"

#include "Jon.h"
#include "Game.h"

#include "NGRect.h"
#include "OverlapTester.h"
#include "EntityUtils.h"

#include <math.h>

Ground* Ground::create(int gridX, int gridY, int type)
{
    GroundType gt = (GroundType)type;
    switch (gt)
    {
        case GroundType_CaveExtraDeepEndLeft:
            return new Ground(gridX, 0, 8, 8, 0, 1, 0.875f, gt, GROUND_SOUND_ID_CAVE);
        case GroundType_CaveExtraDeepSmall:
            return new Ground(gridX, 0, 32, 8, 0, 1, 0.875f, gt, GROUND_SOUND_ID_CAVE);
        case GroundType_CaveExtraDeepMedium:
            return new Ground(gridX, 0, 64, 8, 0, 1, 0.875f, gt, GROUND_SOUND_ID_CAVE);
        case GroundType_CaveExtraDeepLarge:
            return new Ground(gridX, 0, 128, 8, 0, 1, 0.875f, gt, GROUND_SOUND_ID_CAVE);
        case GroundType_CaveExtraDeepEndRight:
            return new Ground(gridX, 0, 8, 8, 0, 1, 0.875f, gt, GROUND_SOUND_ID_CAVE);
            
        case GroundType_CaveDeepEndLeft:
            return new Ground(gridX, 20, 8, 8, 0, 1, 0.875f, gt, GROUND_SOUND_ID_CAVE);
        case GroundType_CaveDeepSmall:
            return new Ground(gridX, 20, 32, 8, 0, 1, 0.875f, gt, GROUND_SOUND_ID_CAVE);
        case GroundType_CaveDeepMedium:
            return new Ground(gridX, 20, 64, 8, 0, 1, 0.875f, gt, GROUND_SOUND_ID_CAVE);
        case GroundType_CaveDeepLarge:
            return new Ground(gridX, 20, 128, 8, 0, 1, 0.875f, gt, GROUND_SOUND_ID_CAVE);
        case GroundType_CaveDeepEndRight:
            return new Ground(gridX, 20, 8, 8, 0, 1, 0.875f, gt, GROUND_SOUND_ID_CAVE);
            
        case GroundType_CaveEndLeft:
            return new Ground(gridX, 50, 8, 24, 0.08333333333333f, 1, 0.20833333333334f, gt, GROUND_SOUND_ID_CAVE);
        case GroundType_CaveSmall:
            return new Ground(gridX, 50, 32, 24, 0.08333333333333f, 1, 0.20833333333334f, gt, GROUND_SOUND_ID_CAVE);
        case GroundType_CaveMedium:
            return new Ground(gridX, 50, 64, 24, 0.08333333333333f, 1, 0.20833333333334f, gt, GROUND_SOUND_ID_CAVE);
        case GroundType_CaveLarge:
            return new Ground(gridX, 50, 128, 24, 0.08333333333333f, 1, 0.20833333333334f, gt, GROUND_SOUND_ID_CAVE);
        case GroundType_CaveEndRight:
            return new Ground(gridX, 50, 8, 24, 0.08333333333333f, 1, 0.20833333333334f, gt, GROUND_SOUND_ID_CAVE);
            
        case GroundType_CaveRaisedEndLeft:
            return new Ground(gridX, 48, 8, 32, 0, 1, 0.84375f, gt, GROUND_SOUND_ID_CAVE);
        case GroundType_CaveRaisedSmall:
            return new Ground(gridX, 48, 32, 32, 0, 1, 0.84375f, gt, GROUND_SOUND_ID_CAVE);
        case GroundType_CaveRaisedMedium:
            return new Ground(gridX, 48, 64, 32, 0, 1, 0.84375f, gt, GROUND_SOUND_ID_CAVE);
        case GroundType_CaveRaisedLarge:
            return new Ground(gridX, 48, 128, 32, 0, 1, 0.84375f, gt, GROUND_SOUND_ID_CAVE);
        case GroundType_CaveRaisedEndRight:
            return new Ground(gridX, 48, 8, 32, 0, 1, 0.84375f, gt, GROUND_SOUND_ID_CAVE);
            
        case GroundType_GrassWithCaveEndLeft:
            return new Ground(gridX, 90, 8, 12, 0.16666666666667f, 1, 0.5f, gt, GROUND_SOUND_ID_GRASS);
        case GroundType_GrassWithCaveSmall:
            return new Ground(gridX, 90, 32, 12, 0.16666666666667f, 1, 0.5f, gt, GROUND_SOUND_ID_GRASS);
        case GroundType_GrassWithCaveMedium:
            return new Ground(gridX, 90, 64, 12, 0.16666666666667f, 1, 0.5f, gt, GROUND_SOUND_ID_GRASS);
        case GroundType_GrassWithCaveLarge:
            return new Ground(gridX, 90, 128, 12, 0.16666666666667f, 1, 0.5f, gt, GROUND_SOUND_ID_GRASS);
        case GroundType_GrassWithCaveEndRight:
            return new Ground(gridX, 90, 8, 12, 0.16666666666667f, 1, 0.5f, gt, GROUND_SOUND_ID_GRASS);
            
        case GroundType_GrassWithoutCaveEndLeft:
            return new Ground(gridX, 0, 8, 100, 0, 1, 0.96f, gt, GROUND_SOUND_ID_GRASS);
        case GroundType_GrassWithoutCaveSmall:
            return new Ground(gridX, 0, 32, 100, 0, 1, 0.96f, gt, GROUND_SOUND_ID_GRASS);
        case GroundType_GrassWithoutCaveMedium:
            return new Ground(gridX, 0, 64, 100, 0, 1, 0.96f, gt, GROUND_SOUND_ID_GRASS);
        case GroundType_GrassWithoutCaveLarge:
            return new Ground(gridX, 0, 128, 100, 0, 1, 0.96f, gt, GROUND_SOUND_ID_GRASS);
        case GroundType_GrassWithoutCaveEndRight:
            return new Ground(gridX, 0, 8, 100, 0, 1, 0.96f, gt, GROUND_SOUND_ID_GRASS);
            
        case GroundType_GrassPitSmall:
            return new GrassPit(gridX, 0, 32, 100, 0, 1, 0.96f, gt, GROUND_SOUND_ID_NONE);
        case GroundType_GrassPitMedium:
            return new GrassPit(gridX, 0, 64, 100, 0, 1, 0.96f, gt, GROUND_SOUND_ID_NONE);
        case GroundType_GrassPitLarge:
            return new GrassPit(gridX, 0, 128, 100, 0, 1, 0.96f, gt, GROUND_SOUND_ID_NONE);
        case GroundType_GrassPitExtraLarge:
            return new GrassPit(gridX, 0, 256, 100, 0, 1, 0.96f, gt, GROUND_SOUND_ID_NONE);
            
        case GroundType_PitTunnelLeft:
            return new PitTunnel(gridX, gridY, 24, 32, 0, 1.5f, 0.234375f, gt, GROUND_SOUND_ID_CAVE);
        case GroundType_PitTunnelCenter:
            return new PitTunnel(gridX, gridY, 32, 32, 0, 1.5f, 0.234375f, gt, GROUND_SOUND_ID_CAVE);
        case GroundType_PitTunnelRight:
            return new PitTunnel(gridX, gridY, 24, 32, 0, 1, 0.234375f, gt, GROUND_SOUND_ID_CAVE);
    }
    
    assert(false);
}

Ground::Ground(int gridX, int gridY, int gridWidth, int gridHeight, float boundsY, float boundsWidth, float boundsHeight, GroundType type, GroundSoundType groundSoundType) : GridLockedPhysicalEntity(gridX, gridY, gridWidth, gridHeight, 0, boundsY, boundsWidth, boundsHeight), m_game(nullptr), m_type(type), m_groundSoundType(groundSoundType)
{
    updateBounds();
}

bool Ground::isEntityLanding(PhysicalEntity* entity, float deltaTime)
{
    float entityVelocityY = entity->getVelocity().getY();
    
    if (entityVelocityY <= 0)
    {
        if (OverlapTester::doNGRectsOverlap(entity->getMainBounds(), getMainBounds()))
        {
            float entityBottom = entity->getMainBounds().getBottom();
            float entityYDelta = fabsf(entityVelocityY * deltaTime);
            
            float itemTop = getMainBounds().getTop();
            float padding = itemTop * .01f;
            padding += entityYDelta;
            float itemTopReq = itemTop - padding;
            
            if (entityBottom >= itemTopReq)
            {
                EntityUtils::placeOn(entity, itemTop);
                
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

int Ground::getEntityLandingPriority()
{
    return 0;
}

bool Ground::isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime)
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

bool Ground::isEntityBlockedOnLeft(PhysicalEntity* entity, float deltaTime)
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

bool Ground::isJonBlockedAbove(Jon &jon, float deltaTime)
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

void Ground::setGame(Game* game)
{
    m_game = game;
}

GrassPit::GrassPit(int gridX, int gridY, int gridWidth, int gridHeight, float boundsY, float boundsWidth, float boundsHeight, GroundType type, GroundSoundType groundSoundType) : Ground(gridX, gridY, gridWidth, gridHeight, boundsY, boundsWidth, boundsHeight, type, groundSoundType)
{
    // Empty
}

bool GrassPit::isEntityLanding(PhysicalEntity* entity, float deltaTime)
{
    return false;
}

int GrassPit::getEntityLandingPriority()
{
    return -1;
}

bool GrassPit::isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime)
{
    for (std::vector<Ground*>::iterator i = m_game->getPits().begin(); i != m_game->getPits().end(); ++i)
    {
        if ((*i)->getRTTI().derivesFrom(PitTunnel::rtti))
        {
            float itemLeft = (*i)->getMainBounds().getLeft();
            float itemRight = (*i)->getMainBounds().getRight();
            float itemBottom = (*i)->getMainBounds().getTop();
            
            NGRect objBounds = NGRect(itemLeft, itemBottom, (*i)->getMainBounds().getWidth(), (*i)->getHeight());
            
            if (OverlapTester::doNGRectsOverlap(entity->getMainBounds(), objBounds)
                && entity->getMainBounds().getRight() < itemRight
                && entity->getMainBounds().getTop() < objBounds.getTop()
                && entity->getMainBounds().getBottom() > objBounds.getBottom() - 0.1f)
            {
                return false;
            }
        }
    }
    
    float entityVelocityX = entity->getVelocity().getX();
    float entityXDelta = entityVelocityX * deltaTime;
    
    NGRect pitBounds = NGRect(getMainBounds().getLeft(), getMainBounds().getBottom(), getMainBounds().getWidth(), getMainBounds().getHeight());
    
    pitBounds.setWidth(pitBounds.getWidth() + entityXDelta);
    
    if (OverlapTester::doNGRectsOverlap(entity->getMainBounds(), pitBounds))
    {
        float entityBottom = entity->getMainBounds().getBottom();
        float entityRight = entity->getMainBounds().getRight();
        
        float itemTop = pitBounds.getTop();
        float itemTopReq = itemTop * 0.99f;
        
        float itemLeftReq = getMainBounds().getRight() - 0.25f;
        
        if (entityRight >= itemLeftReq
            && entityBottom < itemTopReq)
        {
            entity->getPosition().setX(itemLeftReq - entity->getMainBounds().getWidth() / 2 * 1.01f);
            entity->updateBounds();
            
            return true;
        }
    }
    
    return false;
}

bool GrassPit::isEntityBlockedOnLeft(PhysicalEntity* entity, float deltaTime)
{
    for (std::vector<Ground*>::iterator i = m_game->getPits().begin(); i != m_game->getPits().end(); ++i)
    {
        if ((*i)->getRTTI().derivesFrom(PitTunnel::rtti))
        {
            float itemLeft = (*i)->getMainBounds().getLeft();
            float itemBottom = (*i)->getMainBounds().getTop();
            
            NGRect objBounds = NGRect(itemLeft, itemBottom, (*i)->getMainBounds().getWidth(), (*i)->getHeight());
            
            if (OverlapTester::doNGRectsOverlap(entity->getMainBounds(), objBounds)
                && entity->getMainBounds().getLeft() < itemLeft
                && entity->getMainBounds().getTop() < objBounds.getTop()
                && entity->getMainBounds().getBottom() > objBounds.getBottom() - 0.1f)
            {
                return false;
            }
        }
    }
    
	float entityVelocityX = entity->getVelocity().getX();
	float entityXDelta = entityVelocityX * deltaTime;

	NGRect pitBounds = NGRect(getMainBounds().getLeft() - fabsf(entityXDelta), getMainBounds().getBottom(), getMainBounds().getWidth(), getMainBounds().getHeight());

	pitBounds.setWidth(pitBounds.getWidth() + fabsf(entityXDelta));

	if (OverlapTester::doNGRectsOverlap(entity->getMainBounds(), pitBounds))
	{
		float entityBottom = entity->getMainBounds().getBottom();
		float entityLeft = entity->getMainBounds().getLeft();

		float itemTop = pitBounds.getTop();
		float itemTopReq = itemTop * 0.99f;

		float itemRightReq = getMainBounds().getLeft() + 0.25f;

		if (entityLeft <= itemRightReq
			&& entityBottom < itemTopReq)
		{
			entity->getPosition().setX(itemRightReq + entity->getMainBounds().getWidth() / 2 * 1.01f);
			entity->updateBounds();

			return true;
		}
	}

	return false;
}

bool GrassPit::isJonBlockedAbove(Jon& jon, float deltaTime)
{
    return false;
}

bool GrassPit::canObjectBePlacedOn()
{
    return false;
}

bool GrassPit::canObjectBePlacedUnder()
{
    return false;
}

PitTunnel::PitTunnel(int gridX, int gridY, int gridWidth, int gridHeight, float boundsY, float boundsWidth, float boundsHeight, GroundType type, GroundSoundType groundSoundType) : Ground(gridX, gridY, gridWidth, gridHeight, boundsY, boundsWidth, boundsHeight, type, groundSoundType)
{
    // Empty
}

bool PitTunnel::isEntityLanding(PhysicalEntity* entity, float deltaTime)
{
    return Ground::isEntityLanding(entity, deltaTime);
}

int PitTunnel::getEntityLandingPriority()
{
    return 1;
}

bool PitTunnel::isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime)
{
    return false;
}

bool PitTunnel::isEntityBlockedOnLeft(PhysicalEntity* entity, float deltaTime)
{
    return false;
}

bool PitTunnel::isJonBlockedAbove(Jon& jon, float deltaTime)
{
    float entityLeft = jon.getMainBounds().getLeft();
    float entityRight = jon.getMainBounds().getRight();
    float entityTop = jon.getMainBounds().getTop();
    float entityVelocityY = jon.getVelocity().getY();
    
    float itemLeft = getPosition().getX() - getWidth() / 2;
    float itemBottom = getPosition().getY() - getHeight() / 2;
    float itemTop = getPosition().getY() + getHeight() / 2;
    
    NGRect objBounds = NGRect(itemLeft, itemBottom, getWidth(), getHeight());
    
    if (entityVelocityY > 0
        && OverlapTester::doNGRectsOverlap(jon.getMainBounds(), objBounds)
        && entityTop > itemTop
        && entityLeft > objBounds.getLeft() + 0.25f
        && entityRight < objBounds.getRight() - 0.25f)
    {
        jon.getPosition().sub(0, jon.getVelocity().getY() * deltaTime);
        jon.updateBounds();
        
        return true;
    }
    
    return false;
}

bool PitTunnel::canObjectBePlacedOn()
{
    return true;
}

bool PitTunnel::canObjectBePlacedUnder()
{
    return false;
}

RTTI_IMPL(Ground, GridLockedPhysicalEntity);
RTTI_IMPL(GrassPit, Ground);
RTTI_IMPL(PitTunnel, Ground);
