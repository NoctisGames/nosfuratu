//
//  Platform.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/4/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__Platform__
#define __nosfuratu__Platform__

#include "PhysicalEntity.h"

class Platform : public PhysicalEntity
{
public:
    Platform(float x, float y, float width, float height);
};

#endif /* defined(__nosfuratu__Platform__) */
