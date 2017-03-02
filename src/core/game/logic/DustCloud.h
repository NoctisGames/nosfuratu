//
//  DustCloud.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__DustCloud__
#define __nosfuratu__DustCloud__

#include "PhysicalEntity.h"

#include "Color.h"

#include "RTTI.h"

typedef enum
{
    DustCloudType_Landing,
    DustCloudType_Kick_Up
} DustCloudType;

class DustCloud : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    static DustCloud* create(float x, float y, DustCloudType type, float scale = 1);
    
    DustCloud(float x, float y, float width, float height, DustCloudType type);
    
    virtual void update(float deltaTime);
    
    DustCloudType getType() { return m_type; }
    
    Color getColor();
    
private:
    DustCloudType m_type;
    Color m_color;
};

#endif /* defined(__nosfuratu__DustCloud__) */
