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

#include "BackgroundType.h"

#include "RTTI.h"

class Background : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    static Background* create(float x, int type);
    
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

class Water : public Background
{
    RTTI_DECL;
    
public:
    Water(float x, float y, float width, float height, int xScrollSpeed, BackgroundType type);
    
    virtual void update(Vector2D& cameraPosition, float deltaTime);
};

#endif /* defined(__nosfuratu__Background__) */
