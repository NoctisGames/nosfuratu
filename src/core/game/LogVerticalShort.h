//
//  LogVerticalShort.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__LogVerticalShort__
#define __nosfuratu__LogVerticalShort__

#include "DestructiblePhysicalEntity.h"

class LogVerticalShort : public DestructiblePhysicalEntity
{
public:
    static LogVerticalShort* create(float x, float y, int type);
    
    LogVerticalShort(float x, float y, float width = 1.5204678362573099f, float height = 1.216637781629116f);
    
    virtual void updateBounds();
    
    int getType();
};

#endif /* defined(__nosfuratu__LogVerticalShort__) */
