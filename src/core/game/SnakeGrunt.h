//
//  SnakeGrunt.h
//  nosfuratu
//
//  Created by Stephen Gowen on 12/27/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__SnakeGrunt__
#define __nosfuratu__SnakeGrunt__

#include "DestructiblePhysicalEntity.h"
#include "Color.h"

#define SNAKE_GRUNT_WIDTH 3.0681647940074908f
#define SNAKE_GRUNT_HEIGHT 3.0679094540612515f

class SnakeGrunt : public DestructiblePhysicalEntity
{
public:
    static SnakeGrunt* create(float x, float y, int type);
    
    SnakeGrunt(float x, float y, float width = SNAKE_GRUNT_WIDTH, float height = SNAKE_GRUNT_HEIGHT);
    
    virtual void update(float deltaTime);
    
    virtual void triggerHit();
    
    virtual void updateBounds();
    
    Color getColor();
    
    bool isDying();
    
    int getType();
    
private:
    Color m_color;
    bool m_isDying;
};

#endif /* defined(__nosfuratu__SnakeGrunt__) */
