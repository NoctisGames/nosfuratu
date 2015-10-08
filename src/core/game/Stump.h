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
#include "EntityAnchor.h"

#include <vector>

class Stump : public PhysicalEntity
{
public:
    static void create(std::vector<Stump>& items, float x, float y);
    
    Stump(float x, float y, float width = 3.9298245614035086f, float height = 3.3457538994800693f);
    
    virtual void updateBounds();
    
    static Stump deserialize(rapidjson::Value& v);
};

#endif /* defined(__nosfuratu__Stump__) */
