//
//  DustCloud.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "pch.h"

#include "DustCloud.h"

DustCloud* DustCloud::create(float x, float y, DustCloudType type, float scale)
{
    switch (type) {
        case DustCloudType_Landing:
            return new DustCloud(x, y + 1.4038128249566724f / 2 * scale, 2.690058479532164f * scale, 1.4038128249566724f * scale, type);
        case DustCloudType_Kick_Up:
            return new DustCloud(x, y, 0.84375f, 0.28125f, type);
        default:
            break;
    }
}

DustCloud::DustCloud(float x, float y, float width, float height, DustCloudType type) : PhysicalEntity(x, y, width, height), m_type(type)
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

DustCloudType DustCloud::getType()
{
    return m_type;
}

RTTI_IMPL(DustCloud, PhysicalEntity);
