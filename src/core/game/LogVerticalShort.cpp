//
//  LogVerticalShort.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "LogVerticalShort.h"
#include "Rectangle.h"
#include "Vector2D.h"
#include "GameConstants.h"
#include "EntityUtils.h"

#define LOG_VERTICAL_SHORT_WIDTH 1.5204678362573099f
#define LOG_VERTICAL_SHORT_HEIGHT 1.216637781629116f

LogVerticalShort::LogVerticalShort(float x, EntityAnchor anchor) : DestructiblePhysicalEntity(x + LOG_VERTICAL_SHORT_WIDTH / 2, 0, LOG_VERTICAL_SHORT_WIDTH, LOG_VERTICAL_SHORT_HEIGHT)
{
    EntityUtils::applyAnchor(*this, anchor);
    
    updateBounds();
}

void LogVerticalShort::updateBounds()
{
    Vector2D &lowerLeft = m_bounds->getLowerLeft();
    lowerLeft.set(m_position->getX() - getWidth() / 2, m_position->getY() - getHeight() / 2);
    
    lowerLeft.add(getWidth() * 0.18461538461538f, 0);
    
    m_bounds->setWidth(getWidth() * 0.43076923076923f);
    m_bounds->setHeight(getHeight() * 0.90384615384615f);
}