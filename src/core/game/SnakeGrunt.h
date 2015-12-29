//
//  SnakeGrunt.h
//  nosfuratu
//
//  Created by Stephen Gowen on 12/28/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__SnakeGrunt__
#define __nosfuratu__SnakeGrunt__

#include "SnakeEnemy.h"

class SnakeGrunt : public SnakeEnemy
{
public:
    static SnakeGrunt* create(float x, float y, int type);
    
    SnakeGrunt(float x, float y, float width = 3.0681647940074908f, float height = 3.0679094540612515f);
    
    virtual void update(float deltaTime);
    
    virtual void triggerHit();
    
    virtual void updateBounds();
};

#endif /* defined(__nosfuratu__SnakeGrunt__) */
