//
//  UpwardSpike.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "UpwardSpike.h"
#include "EntityUtils.h"

UpwardSpike UpwardSpike::create(float x, UpwardSpikeType type)
{
    switch (type)
    {
        case UpwardSpikeType::UPWARD_SPIKE_METAL_GRASS:
            return UpwardSpike(x, 0.6549707602339181f, 1.0294627383015598f, type, EntityAnchor::ANCHOR_GROUND);
        case UpwardSpikeType::UPWARD_SPIKE_WOOD_GRASS:
            return UpwardSpike(x, 0.9590643274853801f, 1.0762564991334487f, type, EntityAnchor::ANCHOR_GROUND);
        case UpwardSpikeType::UPWARD_SPIKE_METAL_CAVE:
        default:
            return UpwardSpike(x, 0.42105263157894735f, 1.123050259965338f, type, EntityAnchor::ANCHOR_CAVE);
    }
}

UpwardSpike::UpwardSpike(float x, float width, float height, UpwardSpikeType type, EntityAnchor anchor) : PhysicalEntity(x + width / 2, 0, width, height), m_upwardSpikeType(type)
{
    EntityUtils::applyAnchor(*this, anchor);
    
    resetBounds(width * 0.50f, height * 0.66f);
    
    updateBounds();
}

UpwardSpikeType UpwardSpike::getUpwardSpikeType()
{
    return m_upwardSpikeType;
}