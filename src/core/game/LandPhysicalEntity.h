//
//  LandPhysicalEntity.h
//  nosfuratu
//
//  Created by Stephen Gowen on 12/19/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__LandPhysicalEntity__
#define __nosfuratu__LandPhysicalEntity__

#include "PhysicalEntity.h"
#include "GroundSoundType.h"

class LandPhysicalEntity : public PhysicalEntity
{
public:
    LandPhysicalEntity(float x, float y, float width, float height);
    
    virtual GroundSoundType getGroundSoundType();
};

#endif /* defined(__nosfuratu__LandPhysicalEntity__) */
