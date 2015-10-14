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
#include "GameConstants.h"

class BackgroundSky : public PhysicalEntity
{
public:
    static BackgroundSky* create(float x, float y, int type);
    
    BackgroundSky(float x, float y, float width = CAM_WIDTH, float height = 14.038128249566725f);
    
    void update(Vector2D& cameraPosition);
    
    float getX();
    
    float getY();
    
    int getType();
    
private:
    float m_fXOffset;
    float m_fX;
    float m_fY;
};

#endif /* defined(__nosfuratu__BackgroundSky__) */
