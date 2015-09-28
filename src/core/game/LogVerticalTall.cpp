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
    m_bounds->setHeight(LOG_VERTICAL_TALL_HEIGHT * 0.96703296703297f);
}