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

#define LOG_VERTICAL_TALL_WIDTH 1.6608187134502923f
#define LOG_VERTICAL_TALL_HEIGHT 2.1291161178509532f

LogVerticalTall::LogVerticalTall(float x, EntityAnchor anchor) : DestructiblePhysicalEntity(x + LOG_VERTICAL_TALL_WIDTH / 2, 0, LOG_VERTICAL_TALL_WIDTH, LOG_VERTICAL_TALL_HEIGHT)
{
    EntityUtils::applyAnchor(*this, anchor);
    
    updateBounds();
}

void LogVerticalTall::updateBounds()
{
    Vector2D &lowerLeft = m_bounds->getLowerLeft();
    lowerLeft.set(m_position->getX() - getWidth() / 2, m_position->getY() - getHeight() / 2);
    
    lowerLeft.add(getWidth() * 0.1830985915493f, 0);
    
    m_bounds->setWidth(getWidth() * 0.46478873239437f);
    m_bounds->setHeight(getHeight() * 0.96703296703297f);
}