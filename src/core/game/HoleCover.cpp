//
//  HoleCover.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 10/20/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "HoleCover.h"

HoleCover::HoleCover(float x, float y, float width, float height) : DestructiblePhysicalEntity(x, y, width, height), m_isBreaking(false)
{
    // Empty
}

void HoleCover::update(float deltaTime)
{
    if (m_isBreaking)
    {
        m_fStateTime += deltaTime;
        
        if (m_fStateTime > 0.66f)
        {
            m_isRequestingDeletion = true;
        }
    }
}

void HoleCover::triggerHit()
{
    m_isBreaking = true;
}