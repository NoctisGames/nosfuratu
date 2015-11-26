//
//  BackgroundSky.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/22/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "BackgroundSky.h"
#include "EntityUtils.h"
#include "EntityAnchor.h"

BackgroundSky* BackgroundSky::create(float x, float y, int type)
{
    return new BackgroundSky(x, y);
}

BackgroundSky::BackgroundSky(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height), m_fX(0), m_fY(0)
{
    EntityUtils::applyAnchor(*this, ANCHOR_TOP);
    
    int i = -1;
    while (x > 0)
    {
        x -= CAM_WIDTH;
        i++;
    }
    
    m_fXOffset = i * PIXEL_WIDTH_FOR_BACKGROUND;
}

void BackgroundSky::update(Vector2D& cameraPosition)
{
    m_fX = cameraPosition.getX() * 6 + m_fXOffset;
    
    if (m_fX > TEXTURE_SIZE_2048)
    {
        m_fX -= TEXTURE_SIZE_2048;
    }
    
    float y = GAME_HEIGHT - CAM_HEIGHT - cameraPosition.getY();
    y /= (GAME_HEIGHT - CAM_HEIGHT);
    y *= 169;
    y =  y < 0 ? 0 : y;
    
    m_fY = y;
}

float BackgroundSky::getX()
{
    return m_fX;
}

float BackgroundSky::getY()
{
    return m_fY;
}

int BackgroundSky::getType()
{
    return -1;
}