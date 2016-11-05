//
//  Background.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/6/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__Background__
#define __nosfuratu__Background__

#include "PhysicalEntity.h"

#include "GameConstants.h"
#include "RTTI.h"

typedef enum
{
    BackgroundType_Upper,
    BackgroundType_Mid,
    BackgroundType_Lower,
    BackgroundType_WaterBack,
    BackgroundType_WaterFront
} BackgroundType;

class Background : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    Background(float x, float y, float width, float height, int xScrollSpeed, BackgroundType type);
    
    virtual void update(Vector2D& cameraPosition, float deltaTime);
    
    float getTextureRegionX();
    
    BackgroundType getType();
    
protected:
    float m_fXOffset;
    float m_fTextureRegionX;
    int m_iXScrollSpeed;
    BackgroundType m_type;
};

class Upper : public Background
{
    RTTI_DECL;
    
public:
    Upper(float x) : Background(x, 27.0f, CAM_WIDTH, 18.0f, 12, BackgroundType_Upper) {}
};

class Mid : public Background
{
    RTTI_DECL;
    
public:
    Mid(float x) : Background(x, 18.0f, CAM_WIDTH, 9.0f, 24, BackgroundType_Mid) {}
};

class Lower : public Background
{
    RTTI_DECL;
    
public:
    Lower(float x) : Background(x, 7.59375f, CAM_WIDTH, 15.1875f, 36, BackgroundType_Lower) {}
};

class Water : public Background
{
    RTTI_DECL;
    
public:
    Water(float x, float y, float width, float height, int xScrollSpeed, BackgroundType type) : Background(x, y, width, height, xScrollSpeed, type) {}
    
    virtual void update(Vector2D& cameraPosition, float deltaTime);
};

class WaterBack : public Water
{
    RTTI_DECL;
    
public:
    WaterBack(float x) : Water(x, 0.28125f, CAM_WIDTH, 0.5625f, 256, BackgroundType_WaterBack) {}
};

class WaterFront : public Water
{
    RTTI_DECL;
    
public:
    WaterFront(float x) : Water(x, 0.2109375f, CAM_WIDTH, 0.421875f, 320, BackgroundType_WaterFront) {}
};

#endif /* defined(__nosfuratu__Background__) */
