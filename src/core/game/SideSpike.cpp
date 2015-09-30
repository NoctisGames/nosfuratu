//
//  SideSpike.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "SideSpike.h"
#include "EntityUtils.h"

SideSpike SideSpike::create(float x, float y)
{
    return SideSpike(x, y, 1.1228070175438596f, 0.42114384748700173f);
}

SideSpike::SideSpike(float x, float y, float width, float height) : PhysicalEntity(x + width / 2, y, width, height)
{
    resetBounds(width * 0.40f, height * 0.40f);
}