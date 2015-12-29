//
//  SnakeHorned.h
//  nosfuratu
//
//  Created by Stephen Gowen on 12/28/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__SnakeHorned__
#define __nosfuratu__SnakeHorned__

#include "SnakeEnemy.h"

class SnakeHorned : public SnakeEnemy
{
public:
    static SnakeHorned* create(float x, float y, int type);
    
    SnakeHorned(float x, float y, float width = 3.0681647940074908f, float height = 4.314247669773635f);
    
    virtual void update(float deltaTime);
    
    virtual void triggerHit();
    
    virtual void updateBounds();
    
    virtual bool canBeLandedOnToKill();
    
    bool isRising();
    
private:
    bool m_isRising;
};

#endif /* defined(__nosfuratu__SnakeHorned__) */
