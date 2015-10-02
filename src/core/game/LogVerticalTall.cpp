//
//  LogVerticalTall.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/22/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "LogVerticalTall.h"
#include "Rectangle.h"
#include "Vector2D.h"
#include "GameConstants.h"
#include "EntityUtils.h"

void LogVerticalTall::create(std::vector<LogVerticalTall>& items, float x, EntityAnchor anchor)
{
    items.push_back(LogVerticalTall(x, 0));
    
    EntityUtils::applyAnchor(items.at(items.size() - 1), anchor);
    
    items.at(items.size() - 1).updateBounds();
}

LogVerticalTall::LogVerticalTall(float x, float y, float width, float height) : DestructiblePhysicalEntity(x, y, width, height)
{
    updateBounds();
}

void LogVerticalTall::updateBounds()
{
    Vector2D &lowerLeft = m_bounds->getLowerLeft();
    lowerLeft.set(m_position->getX() - getWidth() / 2, m_position->getY() - getHeight() / 2);
    
    lowerLeft.add(getWidth() * 0.18461538461538f, 0);
    
    m_bounds->setWidth(getWidth() * 0.43076923076923f);
    m_bounds->setHeight(getHeight() * 0.90384615384615f);
}

LogVerticalTall LogVerticalTall::deserialize(rapidjson::Value& v)
{
    float x = v[xKey].GetDouble();
    float y = v[ykey].GetDouble();
    float width = v[widthKey].GetDouble();
    float height = v[heightKey].GetDouble();
    
    return LogVerticalTall(x, y, width, height);
}