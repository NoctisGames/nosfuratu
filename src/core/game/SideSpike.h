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

#include <vector>

class SideSpike : public PhysicalEntity
{
public:
    static void create(std::vector<SideSpike>& items, float x, float y);
    
    SideSpike(float x, float y, float width = 1.1228070175438596f, float height = 0.42114384748700173f);
    
    virtual void updateBounds();
    
    static SideSpike deserialize(rapidjson::Value& v);
};

#endif /* defined(__nosfuratu__SideSpike__) */
