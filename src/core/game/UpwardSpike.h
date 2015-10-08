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

#include <vector>

class UpwardSpike : public PhysicalEntity
{
public:
    static void create(std::vector<UpwardSpike>& items, float x, float y, UpwardSpikeType type);
    
    UpwardSpike(float x, float y, float width, float height, UpwardSpikeType type);
    
    virtual void updateBounds();
    
    UpwardSpikeType getUpwardSpikeType();
    
    static UpwardSpike deserialize(rapidjson::Value& v);
    
    virtual void serializeAdditionalParams(rapidjson::Writer<rapidjson::StringBuffer>& w);
    
private:
    UpwardSpikeType m_upwardSpikeType;
};

#endif /* defined(__nosfuratu__UpwardSpike__) */
