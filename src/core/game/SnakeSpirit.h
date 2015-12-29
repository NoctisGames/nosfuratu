//
//  SnakeSpirit.h
//  nosfuratu
//
//  Created by Stephen Gowen on 12/28/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__SnakeSpirit__
#define __nosfuratu__SnakeSpirit__

#include "DestructiblePhysicalEntity.h"
#include "Color.h"

class SnakeSpirit : public DestructiblePhysicalEntity
{
public:
    SnakeSpirit(float x, float y, float width = 3.0681647940074908f, float height = 3.0679094540612515f);
    
    virtual void update(float deltaTime);
};

#endif /* defined(__nosfuratu__SnakeSpirit__) */
