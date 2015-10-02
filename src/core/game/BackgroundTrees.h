//
//  BackgroundTrees.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/22/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__BackgroundTrees__
#define __nosfuratu__BackgroundTrees__

#include "PhysicalEntity.h"

#include <vector>

class BackgroundTrees : public PhysicalEntity
{
public:
    static void create(std::vector<BackgroundTrees>& items, float x);
    
    BackgroundTrees(float x, float y, float width, float height);
    
    void update(Vector2D& cameraPosition);
    
    float getX();
    
    static BackgroundTrees deserialize(rapidjson::Value& v);
    
private:
    float m_fX;
};

#endif /* defined(__nosfuratu__BackgroundTrees__) */
