//
//  EndSign.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/22/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__EndSign__
#define __nosfuratu__EndSign__

#include "PhysicalEntity.h"

class EndSign : public PhysicalEntity
{
public:
    static EndSign* create(int gridX, int gridY, int type);
    
    EndSign(float x, float y, float width = 0.7910112359550562f, float height = 1.114513981358189f);
    
    virtual void updateBounds();
    
    int getType();
};

#endif /* defined(__nosfuratu__EndSign__) */
