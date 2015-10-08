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
#include "EntityAnchor.h"

#include <vector>

class EndSign : public PhysicalEntity
{
public:
    static void create(std::vector<EndSign>& items, float x, float y);
    
    EndSign(float x, float y, float width = 0.7953216374269005f, float height = 1.123050259965338f);
    
    static EndSign deserialize(rapidjson::Value& v);
};

#endif /* defined(__nosfuratu__EndSign__) */
