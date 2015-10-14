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
#include "GameConstants.h"

class BackgroundCave : public PhysicalEntity
{
public:
    static BackgroundCave* create(float x, float y, int type);
    
    BackgroundCave(float x, float y, float width = CAM_WIDTH, float height = 11.277296360485268f);
    
    void update(Vector2D& cameraPosition);
    
    float getX();
    
    int getType();
    
private:
    float m_fXOffset;
    float m_fX;
};

#endif /* defined(__nosfuratu__BackgroundCave__) */
