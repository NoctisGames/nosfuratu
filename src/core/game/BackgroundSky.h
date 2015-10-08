//
//  BackgroundSky.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/22/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__BackgroundSky__
#define __nosfuratu__BackgroundSky__

#include "PhysicalEntity.h"

#include <vector>

class BackgroundSky : public PhysicalEntity
{
public:
    static void create(std::vector<BackgroundSky>& items, float x);
    
    BackgroundSky(float x, float y, float width, float height);
    
    void update(Vector2D& cameraPosition);
    
    float getX();
    
    float getY();
    
    static BackgroundSky deserialize(rapidjson::Value& v);
    
private:
    float m_fXOffset;
    float m_fX;
    float m_fY;
};

#endif /* defined(__nosfuratu__BackgroundSky__) */
