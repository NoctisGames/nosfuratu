//
//  BackgroundCave.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/22/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "BackgroundCave.h"
#include "GameConstants.h"
#include "EntityUtils.h"
#include "EntityAnchor.h"

void BackgroundCave::create(std::vector<BackgroundCave>& items, float x)
{
    items.push_back(BackgroundCave(x, 0, CAM_WIDTH, 11.277296360485268f));
    
    EntityUtils::applyAnchor(items.at(items.size() - 1), ANCHOR_BOTTOM);
}

BackgroundCave::BackgroundCave(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height), m_fX(0)
{
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

BackgroundCave BackgroundCave::deserialize(rapidjson::Value& v)
{
    float x = v[xKey].GetDouble();
    float y = v[ykey].GetDouble();
    float width = v[widthKey].GetDouble();
    float height = v[heightKey].GetDouble();
    
    return BackgroundCave(x, y, width, height);
}