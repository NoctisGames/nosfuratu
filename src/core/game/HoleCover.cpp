//
//  HoleCover.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 10/20/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "HoleCover.h"
#include "GameConstants.h"

HoleCover::HoleCover(float x, float y, float width, float height, HoleCoverType type) : PhysicalEntity(x, y, width, height), m_type(type), m_isBreaking(false)
{
    // Empty
}

void HoleCover::update(float deltaTime)
{
    if (m_isBreaking)
    {
        m_fStateTime += deltaTime;
        
        if (m_fStateTime > 0.50f)
        {
            m_isRequestingDeletion = true;
        }
    }
}

void HoleCover::triggerHit(float jonDownwardsVelocity)
{
    if (jonDownwardsVelocity < RABBIT_GRAVITY)
    {
        m_isRequestingDeletion = true;
    }
    else
    {
        m_isBreaking = true;
    }
}

HoleCoverType HoleCover::getType()
{
    return m_type;
}