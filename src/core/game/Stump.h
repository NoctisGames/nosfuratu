//
//  Stump.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__Stump__
#define __nosfuratu__Stump__

#include "PhysicalEntity.h"
#include "EntityAnchor.h"

class Stump : public PhysicalEntity
{
public:
    Stump(float x, EntityAnchor anchor = EntityAnchor::ANCHOR_GROUND);
    
    virtual void updateBounds();
};

#endif /* defined(__nosfuratu__Stump__) */
