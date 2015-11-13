//
//  CaveExitCover.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 11/2/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "CaveExitCover.h"

CaveExitCover::CaveExitCover(float x, float y, float width, float height) : DestructiblePhysicalEntity(x, y, width, height), m_color(1, 1, 1, 1), m_isBreaking(false)
{
    // Empty
}

void CaveExitCover::update(float deltaTime)
{
    if (m_isBreaking)
    {
        m_fStateTime += deltaTime;
        
        if (m_fStateTime > 0.45f)
        {
            m_color.alpha = 1.00f - (m_fStateTime - 0.45f) * 2;
            
            if (m_color.alpha < 0)
            {
                m_color.alpha = 0;
                m_isRequestingDeletion = true;
            }
        }
    }
}

void CaveExitCover::triggerHit()
{
    m_isBreaking = true;
}

Color CaveExitCover::getColor()
{
    return m_color;
}