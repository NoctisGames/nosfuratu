//
//  GoldenCarrot.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/22/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__GoldenCarrot__
#define __nosfuratu__GoldenCarrot__

#include "PhysicalEntity.h"

class GoldenCarrot : public PhysicalEntity
{
public:
    static GoldenCarrot* create(int gridX, int gridY, int type);
    
    GoldenCarrot(float x, float y, float width = 1.246441947565543f, float height = 1.3422103861517976f);
    
    virtual void update(float deltaTime);
    
    void collect();
    
    bool isCollected();
    
    int getType();
    
private:
    bool m_isCollected;
};

#endif /* defined(__nosfuratu__GoldenCarrot__) */
