//
//  SideSpike.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__SideSpike__
#define __nosfuratu__SideSpike__

#include "PhysicalEntity.h"

class SideSpike : public PhysicalEntity
{
public:
    static SideSpike create(float x, float y);
    
    SideSpike(float x, float y, float width, float height);
};

#endif /* defined(__nosfuratu__SideSpike__) */
