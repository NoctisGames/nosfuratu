//
//  BackgroundSky.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/22/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "BackgroundSky.h"
#include "GameConstants.h"
#include "EntityUtils.h"
#include "EntityAnchor.h"

void BackgroundSky::create(std::vector<BackgroundSky>& items, float x)
{
    items.push_back(BackgroundSky(x, 0, CAM_WIDTH, 14.038128249566725f));
    
    EntityUtils::applyAnchor(items.at(items.size() - 1), ANCHOR_TOP);
}

BackgroundSky::BackgroundSky(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height), m_fX(0), m_fY(0)
{
    // Empty
}

void BackgroundSky::update(Vector2D& cameraPosition)
{
    m_fX = cameraPosition.getX() * 6;
    
    float y = cameraPosition.getY();
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

BackgroundSky BackgroundSky::deserialize(rapidjson::Value& v)
{
    float x = v[xKey].GetDouble();
    float y = v[ykey].GetDouble();
    float width = v[widthKey].GetDouble();
    float height = v[heightKey].GetDouble();
    
    return BackgroundSky(x, y, width, height);
}