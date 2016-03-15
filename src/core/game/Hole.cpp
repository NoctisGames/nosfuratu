//
//  Hole.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 10/20/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "Hole.h"
#include "EntityUtils.h"

Hole* Hole::create(int gridX, int gridY, int type)
{
    switch ((HoleType)type)
    {
        case HoleType_GrassTileLeft:
            return new HoleGrassTileLeft(gridX);
        case HoleType_GrassTileCenter:
            return new HoleGrassTileCenter(gridX);
        case HoleType_GrassTileRight:
            return new HoleGrassTileRight(gridX);
        case HoleType_Grass:
            return new HoleGrass(gridX);
        case HoleType_Cave:
            return new HoleCave(gridX);
    }
    
    assert(false);
}

Hole::Hole(int gridX, int gridY, int gridWidth, int gridHeight, HoleType type, HoleCoverType holeCoverType) : GridLockedPhysicalEntity(gridX, gridY, gridWidth, gridHeight, 0, 0, 1, 1.1f), m_holeCover(nullptr), m_type(type)
{
    m_holeCover = new HoleCover(m_position->getX(), m_position->getY(), m_fWidth, m_fHeight, holeCoverType);
}

void Hole::update(float deltaTime)
{
    PhysicalEntity::update(deltaTime);
    
    if (m_holeCover)
    {
        m_holeCover->update(deltaTime);
        m_holeCover->getPosition().setX(getPosition().getX());
        m_holeCover->updateBounds();
        
        if (m_holeCover->isRequestingDeletion())
        {
            delete m_holeCover;
            m_holeCover = nullptr;
        }
    }
}

bool Hole::triggerBurrow(float jonDownwardsVelocity)
{
    if (m_holeCover)
    {
        m_holeCover->triggerHit(jonDownwardsVelocity);
        return true;
    }
    
    return false;
}

bool Hole::hasCover()
{
    return m_holeCover ? true : false;
}

HoleCover& Hole::getHoleCover()
{
    return *m_holeCover;
}

HoleType Hole::getType()
{
    return m_type;
}