//
//  LogVerticalTall.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/22/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__LogVerticalTall__
#define __nosfuratu__LogVerticalTall__

#include "DestructiblePhysicalEntity.h"
#include "EntityAnchor.h"

class LogVerticalTall : public DestructiblePhysicalEntity
{
public:
    LogVerticalTall(float x, EntityAnchor anchor = EntityAnchor::ANCHOR_GROUND);
};

#endif /* defined(__nosfuratu__LogVerticalTall__) */
