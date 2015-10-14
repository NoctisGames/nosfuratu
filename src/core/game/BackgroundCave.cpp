//
//  BackgroundCave.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/22/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "BackgroundCave.h"
#include "EntityUtils.h"
#include "EntityAnchor.h"

BackgroundCave* BackgroundCave::create(float x, float y, int type)
{
    return new BackgroundCave(x, y);
}

BackgroundCave::BackgroundCave(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height), m_fX(0)
{
    EntityUtils::applyAnchor(*this, ANCHOR_BOTTOM);
    
    int i = -1;
    while (x > 0)
    {
        x -= CAM_WIDTH;
        i++;
    }
    
    m_fXOffset = i * PIXEL_WIDTH_FOR_GAME;
}

void BackgroundCave::update(Vector2D& cameraPosition)
{
    m_fX = cameraPosition.getX() * 24 + m_fXOffset;
    
    if (m_fX > TEXTURE_SIZE_2048)
    {
        m_fX -= TEXTURE_SIZE_2048;
    }
}

float BackgroundCave::getX()
{
    return m_fX;
}

int BackgroundCave::getType()
{
    return -1;
}