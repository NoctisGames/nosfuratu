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
    static GoldenCarrot* create(float x, float y, int type);
    
    GoldenCarrot(float x, float y, float width = 1.246441947565543f, float height = 1.174434087882823f);
    
    int getType();
};

#endif /* defined(__nosfuratu__GoldenCarrot__) */
