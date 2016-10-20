//
//  DustCloud.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "DustCloud.h"

#define DUST_CLOUD_HEIGHT 1.4038128249566724f

DustCloud::DustCloud(float x, float y, float scale) : PhysicalEntity(x, y + DUST_CLOUD_HEIGHT / 2 * scale, 2.690058479532164f * scale, DUST_CLOUD_HEIGHT * scale), m_color(1, 1, 1, 1)
{
    // Empty
}

void DustCloud::update(float deltaTime)
{
    PhysicalEntity::update(deltaTime);
    
    if (m_fStateTime > 0.4f)
    {
        m_color.alpha = 1.8 - m_fStateTime * 2;
        
        if (m_color.alpha < 0)
        {
            m_color.alpha = 0;
            m_isRequestingDeletion = true;
        }
    }
}

Color DustCloud::getColor()
{
    return m_color;
}