//
//  BackgroundTrees.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/22/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "BackgroundTrees.h"
#include "GameConstants.h"

void BackgroundTrees::create(std::vector<BackgroundTrees>& items, float x)
{
    items.push_back(BackgroundTrees(x, 14.962305025996535f, CAM_WIDTH, 11.909012131715771f));
}

BackgroundTrees::BackgroundTrees(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height), m_fX(0)
{
    // Empty
}

void BackgroundTrees::update(Vector2D& cameraPosition)
{
    m_fX = cameraPosition.getX() * 12;
}

float BackgroundTrees::getX()
{
    return m_fX;
}

BackgroundTrees BackgroundTrees::deserialize(rapidjson::Value& v)
{
    float x = v[xKey].GetDouble();
    float y = v[ykey].GetDouble();
    float width = v[widthKey].GetDouble();
    float height = v[heightKey].GetDouble();
    
    return BackgroundTrees(x, y, width, height);
}