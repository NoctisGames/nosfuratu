//
//  Thorns.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__Thorns__
#define __nosfuratu__Thorns__

#include "PhysicalEntity.h"

class Thorns : public PhysicalEntity
{
public:
    static Thorns* create(int gridX, int gridY, int type);
    
    Thorns(float x, float y, float width = 2.8284644194756554f, float height = 1.4500665778961384f);
    
    virtual void updateBounds();
    
    int getType();
};

#endif /* defined(__nosfuratu__Thorns__) */
