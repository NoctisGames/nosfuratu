//
//  Hole.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 10/20/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "Hole.h"
#include "EntityUtils.h"

Hole* Hole::create(int gridX, int gridY, int type)
{
    switch ((HoleType)type)
    {
        case HoleType_Grass:
            return new HoleGrass(gridX);
        case HoleType_Cave:
            return new HoleCave(gridX);
    }
    
    assert(false);
}

Hole::Hole(int gridX, int gridY, int gridWidth, int gridHeight, HoleType type) : GridLockedPhysicalEntity(gridX, gridY, gridWidth, gridHeight), m_holeCover(nullptr), m_type(type)
{
    m_holeCover = new HoleCover(m_position->getX(), m_position->getY(), m_fWidth, m_fHeight, type == HoleType_Grass ? HoleCoverType_Grass : HoleCoverType_Cave);
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

bool Hole::triggerBurrow()
{
    if (m_holeCover)
    {
        m_holeCover->triggerHit();
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