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
    BackgroundType_Mid_Hills,
    BackgroundType_Lower_Innermost,
    BackgroundType_Mid_Trees,
    BackgroundType_Lower_Inner,
    BackgroundType_Lower_Top,
    BackgroundType_Lower_Bottom,
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
    Upper(float x) : Background(x, 29.25f, CAM_WIDTH, 13.5f, 12, BackgroundType_Upper) {}
};

class MidHills : public Background
{
    RTTI_DECL;
    
public:
    MidHills(float x) : Background(x, 18.966796875f, CAM_WIDTH, 7.06640625f, 12, BackgroundType_Mid_Hills) {}
};

class LowerInnermost : public Background
{
    RTTI_DECL;
    
public:
    LowerInnermost(float x) : Background(x, 6.5830078125f, CAM_WIDTH, 8.296875f, 12, BackgroundType_Lower_Innermost) {}
};

class MidTrees : public Background
{
    RTTI_DECL;
    
public:
    MidTrees(float x) : Background(x, 16.734375f, CAM_WIDTH, 6.46875f, 24, BackgroundType_Mid_Trees) {}
};

class LowerInner : public Background
{
    RTTI_DECL;
    
public:
    LowerInner(float x) : Background(x, 6.5830078125f, CAM_WIDTH, 8.296875f, 24, BackgroundType_Lower_Inner) {}
};

class LowerTop : public Background
{
    RTTI_DECL;
    
public:
    LowerTop(float x) : Background(x, 12.849609375f, CAM_WIDTH, 8.61328125f, 48, BackgroundType_Lower_Top) {}
};

class LowerBottom : public Background
{
    RTTI_DECL;
    
public:
    LowerBottom(float x) : Background(x, 2.302734375f, CAM_WIDTH, 4.60546875f, 48, BackgroundType_Lower_Bottom) {}
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
