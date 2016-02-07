//
//  Background.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/6/16.
//  Copyright (c) 2016 Gowen Game Dev. All rights reserved.
//

#include "Background.h"

Background::Background(float x, float y, float width, float height, int xScrollSpeed, int type) : PhysicalEntity(x, y, width, height), m_fTextureRegionX(0), m_iXScrollSpeed(xScrollSpeed), m_iType(type)
{
    int i = -1;
    while (x > 0)
    {
        x -= width;
        i++;
    }
    
    m_fXOffset = i * PIXEL_WIDTH_FOR_BACKGROUND;
}

void Background::update(Vector2D& cameraPosition)
{
    m_fTextureRegionX = cameraPosition.getX() * m_iXScrollSpeed + m_fXOffset;
    
    if (m_fTextureRegionX > TEXTURE_SIZE_2048)
    {
        m_fTextureRegionX -= TEXTURE_SIZE_2048;
    }
}

float Background::getTextureRegionX()
{
    return m_fTextureRegionX;
}

int Background::getType()
{
    return m_iType;
}