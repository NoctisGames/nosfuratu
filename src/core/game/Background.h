//
//  Background.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/6/16.
//  Copyright (c) 2016 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__Background__
#define __nosfuratu__Background__

#include "PhysicalEntity.h"
#include "GameConstants.h"

class Background : public PhysicalEntity
{
public:
    Background(float x, float y, float width, float height, int xScrollSpeed, int type);
    
    void update(Vector2D& cameraPosition);
    
    float getTextureRegionX();
    
    int getType();
    
private:
    float m_fXOffset;
    float m_fTextureRegionX;
    int m_iXScrollSpeed;
    int m_iType;
};

class BackgroundUpper : public Background
{
public:
    BackgroundUpper(float x) : Background(x, 27.0f, CAM_WIDTH, 18.0f, 6, 0) {}
};

class BackgroundMid : public Background
{
public:
    BackgroundMid(float x) : Background(x, 18.0f, CAM_WIDTH, 9.0f, 18, 1) {}
};

class BackgroundLower : public Background
{
public:
    BackgroundLower(float x) : Background(x, 7.59375f, CAM_WIDTH, 15.1875f, 32, 2) {}
};

#endif /* defined(__nosfuratu__Background__) */
