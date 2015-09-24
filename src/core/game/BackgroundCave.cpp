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

BackgroundCave::BackgroundCave(float x) : PhysicalEntity(x, 0, CAM_WIDTH, 11.277296360485268f)
{
    EntityUtils::applyAnchor(*this, ANCHOR_BOTTOM);
}