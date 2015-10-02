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
#include "EntityAnchor.h"

#include <vector>

class Thorns : public PhysicalEntity
{
public:
    static void create(std::vector<Thorns>& items, float x, EntityAnchor anchor = EntityAnchor::ANCHOR_GROUND);
    
    Thorns(float x, float y, float width = 2.853801169590643f, float height = 1.474003466204506f);
    
    static Thorns deserialize(rapidjson::Value& v);
};

#endif /* defined(__nosfuratu__Thorns__) */
