//
//  ExitGroundCover.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/9/16.
//  Copyright © 2016 Gowen Game Dev. All rights reserved.
//

#include "ExitGroundCover.h"

ExitGroundCover::ExitGroundCover(float x, float y, float width, float height, ExitGroundCoverType type) : PhysicalEntity(x, y, width, height), m_type(type), m_color(1, 1, 1, 1), m_isBreaking(false)
{
    // Empty
}

void ExitGroundCover::update(float deltaTime)
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

void ExitGroundCover::triggerHit()
{
    m_isBreaking = true;
}

ExitGroundCoverType ExitGroundCover::getType()
{
    return m_type;
}

Color ExitGroundCover::getColor()
{
    return m_color;
}