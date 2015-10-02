//
//  BackgroundCave.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/22/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__BackgroundCave__
#define __nosfuratu__BackgroundCave__

#include "PhysicalEntity.h"

#include <vector>

class BackgroundCave : public PhysicalEntity
{
public:
    static void create(std::vector<BackgroundCave>& items, float x);
    
    BackgroundCave(float x, float y, float width, float height);
    
    void update(Vector2D& cameraPosition);
    
    float getX();
    
    static BackgroundCave deserialize(rapidjson::Value& v);
    
private:
    float m_fX;
};

#endif /* defined(__nosfuratu__BackgroundCave__) */
