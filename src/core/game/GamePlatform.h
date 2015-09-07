//
//  GamePlatform.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/4/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__GamePlatform__
#define __nosfuratu__GamePlatform__

#include "PhysicalEntity.h"

class GamePlatform : public PhysicalEntity
{
public:
    GamePlatform(float x, float y, float width, float height);
};

#endif /* defined(__nosfuratu__GamePlatform__) */
