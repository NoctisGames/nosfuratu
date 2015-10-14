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
#include "GameConstants.h"

class BackgroundTrees : public PhysicalEntity
{
public:
    static BackgroundTrees* create(float x, float y, int type);
    
    BackgroundTrees(float x, float y = 14.962305025996535f, float width = CAM_WIDTH, float height = 11.909012131715771f);
    
    void update(Vector2D& cameraPosition);
    
    float getX();
    
    int getType();
    
private:
    float m_fXOffset;
    float m_fX;
};

#endif /* defined(__nosfuratu__BackgroundTrees__) */
