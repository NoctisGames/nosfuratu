//
//  Carrot.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/4/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__Carrot__
#define __nosfuratu__Carrot__

#include "PhysicalEntity.h"

class Carrot : public PhysicalEntity
{
public:
    static Carrot* create(int gridX, int gridY, int type);
    
    Carrot(float x, float y, float width = 1.2224719101123596f, float height = 1.1025299600532623f);
    
    virtual void update(float deltaTime);
    
    void collect();
    
    bool isCollected();
    
    int getType();
    
private:
    bool m_isCollected;
};

#endif /* defined(__nosfuratu__Carrot__) */
