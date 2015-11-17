//
//  Stump.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__Stump__
#define __nosfuratu__Stump__

#include "PhysicalEntity.h"

class Stump : public PhysicalEntity
{
public:
    static Stump* create(float x, float y, int type);
    
    Stump(float x, float y, float width = 3.8951310861423223f, float height = 3.355525965379494f);
    
    virtual void updateBounds();
    
    int getType();
};

#endif /* defined(__nosfuratu__Stump__) */
