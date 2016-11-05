//
//  Ground.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/5/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
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
            
        case GroundType_GrassPitSmall:
            return new GrassPitSmall(gridX);
        case GroundType_GrassPitMedium:
            return new GrassPitMedium(gridX);
        case GroundType_GrassPitLarge:
            return new GrassPitLarge(gridX);
        case GroundType_GrassPitExtraLarge:
            return new GrassPitExtraLarge(gridX);
    }
    
    assert(false);
}

Ground::Ground(int gridX, int gridY, int gridWidth, int gridHeight, float boundsY, float boundsHeight, GroundType type, GroundSoundType groundSoundType) : GridLockedPhysicalEntity(gridX, gridY, gridWidth, gridHeight, 0, boundsY, 1, boundsHeight), m_type(type), m_groundSoundType(groundSoundType)
{
    updateBounds();
}

bool Ground::isEntityLanding(PhysicalEntity* entity, float deltaTime)
{
    float entityVelocityY = entity->getVelocity().getY();
    
    if (entityVelocityY <= 0)
    {
        if (OverlapTester::doRectanglesOverlap(entity->getMainBounds(), getMainBounds()))
        {
            float entityLowerLeftY = entity->getMainBounds().getLowerLeft().getY();
            float entityYDelta = fabsf(entityVelocityY * deltaTime);
            
            float itemTop = getMainBounds().getTop();
            float padding = itemTop * .01f;
            padding += entityYDelta;
            float itemTopReq = itemTop - padding;
            
            if (entityLowerLeftY >= itemTopReq)
            {
                entity->placeOn(itemTop);
                
                Jon *jon = nullptr;
                if (entity->GetRTTI().DerivesFrom(Jon::rtti))
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

bool Ground::isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime)
{
    if (OverlapTester::doRectanglesOverlap(entity->getMainBounds(), getMainBounds()))
    {
        float entityVelocityX = entity->getVelocity().getX();
        float entityBottom = entity->getMainBounds().getLowerLeft().getY();
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
	if (OverlapTester::doRectanglesOverlap(entity->getMainBounds(), getMainBounds()))
	{
		float entityVelocityX = entity->getVelocity().getX();
		float entityBottom = entity->getMainBounds().getLowerLeft().getY();
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

bool GrassPit::isEntityBlockedOnRight(PhysicalEntity* entity, float deltaTime)
{
    float entityVelocityX = entity->getVelocity().getX();
    float entityXDelta = entityVelocityX * deltaTime;
    
    Rectangle pitBounds = Rectangle(getMainBounds().getLeft(), getMainBounds().getBottom(), getMainBounds().getWidth(), getMainBounds().getHeight());
    
    pitBounds.setWidth(pitBounds.getWidth() + entityXDelta);
    
    if (OverlapTester::doRectanglesOverlap(entity->getMainBounds(), pitBounds))
    {
        float entityBottom = entity->getMainBounds().getLowerLeft().getY();
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
	float entityVelocityX = entity->getVelocity().getX();
	float entityXDelta = entityVelocityX * deltaTime;

	Rectangle pitBounds = Rectangle(getMainBounds().getLeft() - fabsf(entityXDelta), getMainBounds().getBottom(), getMainBounds().getWidth(), getMainBounds().getHeight());

	pitBounds.setWidth(pitBounds.getWidth() + fabsf(entityXDelta));

	if (OverlapTester::doRectanglesOverlap(entity->getMainBounds(), pitBounds))
	{
		float entityBottom = entity->getMainBounds().getLowerLeft().getY();
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

bool GrassPit::canObjectBePlacedOn()
{
    return false;
}

bool GrassPit::canObjectBePlacedUnder()
{
    return false;
}
