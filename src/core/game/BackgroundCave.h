//
//  BackgroundCave.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/22/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__BackgroundCave__
#define __nosfuratu__BackgroundCave__

#include "PhysicalEntity.h"

class Game;

class BackgroundCave : public PhysicalEntity
{
public:
    BackgroundCave(float x);
    
    void update(Game& game);
    
    float getX();
    
private:
    float m_fX;
};

#endif /* defined(__nosfuratu__BackgroundCave__) */
