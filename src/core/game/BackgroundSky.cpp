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

BackgroundSky::BackgroundSky(float x) : PhysicalEntity(x, 0, CAM_WIDTH, 14.038128249566725f)
{
    EntityUtils::applyAnchor(*this, ANCHOR_TOP);
}