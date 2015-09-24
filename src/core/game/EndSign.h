//
//  EndSign.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/22/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__EndSign__
#define __nosfuratu__EndSign__

#include "PhysicalEntity.h"
#include "EntityAnchor.h"

class EndSign : public PhysicalEntity
{
public:
    EndSign(float x, EntityAnchor anchor = EntityAnchor::ANCHOR_GROUND);
};

#endif /* defined(__nosfuratu__EndSign__) */
