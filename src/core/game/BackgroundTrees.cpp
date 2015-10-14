//
//  BackgroundTrees.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/22/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "BackgroundTrees.h"

BackgroundTrees* BackgroundTrees::create(float x, float y, int type)
{
    return new BackgroundTrees(x);
}

BackgroundTrees::BackgroundTrees(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height), m_fX(0)
{
    int i = -1;
    while (x > 0)
    {
        x -= CAM_WIDTH;
        i++;
    }
    
    m_fXOffset = i * PIXEL_WIDTH_FOR_GAME;
}

void BackgroundTrees::update(Vector2D& cameraPosition)
{
    m_fX = cameraPosition.getX() * 12 + m_fXOffset;
    
    if (m_fX > TEXTURE_SIZE_2048)
    {
        m_fX -= TEXTURE_SIZE_2048;
    }
}

float BackgroundTrees::getX()
{
    return m_fX;
}

int BackgroundTrees::getType()
{
    return -1;
}