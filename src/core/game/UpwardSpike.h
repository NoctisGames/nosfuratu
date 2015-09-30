//
//  UpwardSpike.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__UpwardSpike__
#define __nosfuratu__UpwardSpike__

#include "PhysicalEntity.h"
#include "UpwardSpikeType.h"
#include "EntityAnchor.h"

class UpwardSpike : public PhysicalEntity
{
public:
    static UpwardSpike create(float x, UpwardSpikeType type);
    
    UpwardSpike(float x, float width, float height, UpwardSpikeType type, EntityAnchor anchor = EntityAnchor::ANCHOR_NONE);
    
    UpwardSpikeType getUpwardSpikeType();
    
private:
    UpwardSpikeType m_upwardSpikeType;
};

#endif /* defined(__nosfuratu__UpwardSpike__) */
