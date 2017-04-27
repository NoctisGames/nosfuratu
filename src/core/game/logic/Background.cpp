//
//  Background.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/6/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "pch.h"

#include "Background.h"

#include "EntityUtils.h"
#include "GameConstants.h"

Background* Background::create(float x, int type)
{
    BackgroundType bt = (BackgroundType)type;
    switch (bt)
    {
        case BackgroundType_Upper:
            return new Background(x, 29.25f, CAM_WIDTH, 13.5f, 8, bt);
        case BackgroundType_Mid_Hills:
            return new Background(x, 18.966796875f, CAM_WIDTH, 7.06640625f, 8, bt);
        case BackgroundType_Lower_Innermost:
            return new Background(x, 6.5830078125f, CAM_WIDTH, 8.296875f, 8, bt);
        case BackgroundType_Mid_Trees:
            return new Background(x, 16.734375f, CAM_WIDTH, 6.46875f, 16, bt);
        case BackgroundType_Lower_Inner:
            return new Background(x, 6.5830078125f, CAM_WIDTH, 8.296875f, 16, bt);
        case BackgroundType_Lower_Top:
            return new Background(x, 12.849609375f, CAM_WIDTH, 8.61328125f, 32, bt);
        case BackgroundType_Lower_Bottom:
            return new Background(x, 2.302734375f, CAM_WIDTH, 4.60546875f, 32, bt);
        case BackgroundType_WaterBack:
            return new Water(x, 0.28125f, CAM_WIDTH, 0.5625f, 256, bt);
        case BackgroundType_WaterFront:
            return new Water(x, 0.2109375f, CAM_WIDTH, 0.421875f, 320, bt);
    }
    
    assert(false);
}

Background::Background(float x, float y, float width, float height, int xScrollSpeed, BackgroundType type) : PhysicalEntity(x, y, width, height), m_fTextureRegionX(0), m_iXScrollSpeed(xScrollSpeed), m_type(type)
{
    int i = -1;
    while (x > 0)
    {
        x -= width;
        ++i;
    }
    
    m_fXOffset = i * PIXEL_WIDTH_FOR_GAME;
}

void Background::update(Vector2D& cameraPosition, float deltaTime)
{
    m_fTextureRegionX = cameraPosition.getX() * m_iXScrollSpeed + m_fXOffset;
    
    while (m_fTextureRegionX > 2048)
    {
        m_fTextureRegionX -= 2048;
    }
}

float Background::getTextureRegionX()
{
    return m_fTextureRegionX;
}

BackgroundType Background::getType()
{
    return m_type;
}

Water::Water(float x, float y, float width, float height, int xScrollSpeed, BackgroundType type) : Background(x, y, width, height, xScrollSpeed, type)
{
    // Empty
}

void Water::update(Vector2D& cameraPosition, float deltaTime)
{
    m_fStateTime += deltaTime;
    
    m_fTextureRegionX = m_fXOffset + (m_iXScrollSpeed * m_fStateTime);
    
    while (m_fTextureRegionX > 2048)
    {
        m_fTextureRegionX -= 2048;
    }
}

RTTI_IMPL(Background, PhysicalEntity);
RTTI_IMPL(Water, Background);
