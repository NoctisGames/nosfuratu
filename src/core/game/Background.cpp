//
//  Background.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/6/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "Background.h"

Background::Background(float x, float y, float width, float height, int xScrollSpeed, BackgroundType type) : PhysicalEntity(x, y, width, height), m_fTextureRegionX(0), m_iXScrollSpeed(xScrollSpeed), m_type(type)
{
    int i = -1;
    while (x > 0)
    {
        x -= width;
        i++;
    }
    
    m_fXOffset = i * PIXEL_WIDTH_FOR_GAME;
}

void Background::update(Vector2D& cameraPosition, float deltaTime)
{
    m_fTextureRegionX = cameraPosition.getX() * m_iXScrollSpeed + m_fXOffset;
    
    while (m_fTextureRegionX > TEXTURE_SIZE_2048)
    {
        m_fTextureRegionX -= TEXTURE_SIZE_2048;
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

void Water::update(Vector2D& cameraPosition, float deltaTime)
{
    m_fStateTime += deltaTime;
    
    m_fTextureRegionX = m_fXOffset + (m_iXScrollSpeed * m_fStateTime);
    
    while (m_fTextureRegionX > TEXTURE_SIZE_2048)
    {
        m_fTextureRegionX -= TEXTURE_SIZE_2048;
    }
}

RTTI_IMPL(Background, PhysicalEntity);
RTTI_IMPL(Upper, Background);
RTTI_IMPL(MidHills, Background);
RTTI_IMPL(LowerInnermost, Background);
RTTI_IMPL(MidTrees, Background);
RTTI_IMPL(LowerInner, Background);
RTTI_IMPL(LowerTop, Background);
RTTI_IMPL(LowerBottom, Background);
RTTI_IMPL(Water, Background);
RTTI_IMPL(WaterBack, Water);
RTTI_IMPL(WaterFront, Water);
