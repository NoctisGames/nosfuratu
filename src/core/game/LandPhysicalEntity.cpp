//
//  LandPhysicalEntity.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 12/19/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "LandPhysicalEntity.h"

LandPhysicalEntity::LandPhysicalEntity(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height)
{
    // Empty
}

GroundSoundType LandPhysicalEntity::getGroundSoundType()
{
    return GROUND_SOUND_NONE;
}