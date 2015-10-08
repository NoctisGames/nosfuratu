//
//  DestructiblePhysicalEntity.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/28/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "DestructiblePhysicalEntity.h"

DestructiblePhysicalEntity::DestructiblePhysicalEntity(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height)
{
    // Empty
}

void DestructiblePhysicalEntity::triggerHit()
{
    m_isRequestingDeletion = true;
}