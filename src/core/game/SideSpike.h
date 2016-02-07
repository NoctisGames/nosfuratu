//
//  SideSpike.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__SideSpike__
#define __nosfuratu__SideSpike__

#include "PhysicalEntity.h"

class SideSpike : public PhysicalEntity
{
public:
    static SideSpike* create(int gridX, int gridY, int type);
    
    SideSpike(float x, float y, float width = 1.1146067415730336f, float height = 0.4194407456724368f);
    
    virtual void updateBounds();
    
    int getType();
};

#endif /* defined(__nosfuratu__SideSpike__) */
