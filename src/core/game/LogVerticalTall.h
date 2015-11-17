//
//  LogVerticalTall.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/22/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__LogVerticalTall__
#define __nosfuratu__LogVerticalTall__

#include "DestructiblePhysicalEntity.h"

class LogVerticalTall : public DestructiblePhysicalEntity
{
public:
    static LogVerticalTall* create(float x, float y, int type);
    
    LogVerticalTall(float x, float y, float width = 1.6299625468164793f, float height = 2.1091877496671105f);
    
    virtual void updateBounds();
    
    int getType();
};

#endif /* defined(__nosfuratu__LogVerticalTall__) */
