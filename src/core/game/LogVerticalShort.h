//
//  LogVerticalShort.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__LogVerticalShort__
#define __nosfuratu__LogVerticalShort__

#include "DestructiblePhysicalEntity.h"
#include "EntityAnchor.h"

class LogVerticalShort : public DestructiblePhysicalEntity
{
public:
    LogVerticalShort(float x, EntityAnchor anchor = EntityAnchor::ANCHOR_GROUND);
    
    virtual void updateBounds();
};

#endif /* defined(__nosfuratu__LogVerticalShort__) */
