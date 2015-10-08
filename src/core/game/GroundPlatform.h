//
//  GroundPlatform.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/4/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__GroundPlatform__
#define __nosfuratu__GroundPlatform__

#include "PhysicalEntity.h"
#include "GroundPlatformType.h"

#include <vector>

class GroundPlatform : public PhysicalEntity
{
public:
    static void create(std::vector<GroundPlatform>& items, float x, float y, GroundPlatformType type);
    
    GroundPlatform(float x, float y, float width, float height, GroundPlatformType type);
    
    virtual void updateBounds();
    
    GroundPlatformType getGroundPlatformType();
    
    static GroundPlatform deserialize(rapidjson::Value& v);
    
    virtual void serializeAdditionalParams(rapidjson::Writer<rapidjson::StringBuffer>& w);
    
private:
    GroundPlatformType m_groundPlatformType;
};

#endif /* defined(__nosfuratu__GroundPlatform__) */
