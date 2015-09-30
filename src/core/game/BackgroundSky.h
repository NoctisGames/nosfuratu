//
//  BackgroundSky.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/22/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__BackgroundSky__
#define __nosfuratu__BackgroundSky__

#include "PhysicalEntity.h"

class Game;

class BackgroundSky : public PhysicalEntity
{
public:
    BackgroundSky(float x);
    
    void update(Game& game);
    
    float getX();
    
    float getY();
    
private:
    float m_fX;
    float m_fY;
};

#endif /* defined(__nosfuratu__BackgroundSky__) */
