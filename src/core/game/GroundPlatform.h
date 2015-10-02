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
    static void createGrass(std::vector<GroundPlatform>& platforms, float x, float y, int length);
    
    static void createCave(std::vector<GroundPlatform>& platforms, float x, float y, int length);
    
    static void create(std::vector<GroundPlatform>& items, float x, float y, GroundPlatformType type);
    
    GroundPlatform(float x, float y, float width, float height, GroundPlatformType type);
    
    GroundPlatformType getGroundPlatformType();
    
    static GroundPlatform deserialize(rapidjson::Value& v);
    
    virtual void serializeAdditionalParams(rapidjson::Writer<rapidjson::StringBuffer>& w);
    
private:
    static void create(std::vector<GroundPlatform>& platforms, float x, float y, int length, GroundPlatformType typeLeft, GroundPlatformType typeCenter, GroundPlatformType typeRight);
    
    GroundPlatformType m_groundPlatformType;
};

#endif /* defined(__nosfuratu__GroundPlatform__) */
