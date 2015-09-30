//
//  Thorns.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__Thorns__
#define __nosfuratu__Thorns__

#include "PhysicalEntity.h"
#include "EntityAnchor.h"

class Thorns : public PhysicalEntity
{
public:
    Thorns(float x, EntityAnchor anchor = EntityAnchor::ANCHOR_GROUND);
};

#endif /* defined(__nosfuratu__Thorns__) */
