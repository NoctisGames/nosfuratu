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
    static EndSign* create(float x, float y, int type);
    
    EndSign(float x, float y, float width = 0.7953216374269005f, float height = 1.123050259965338f);
    
    int getType();
};

#endif /* defined(__nosfuratu__EndSign__) */
