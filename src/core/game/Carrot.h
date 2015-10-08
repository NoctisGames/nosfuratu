//
//  Carrot.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/4/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__Carrot__
#define __nosfuratu__Carrot__

#include "PhysicalEntity.h"

#include <vector>

class Carrot : public PhysicalEntity
{
public:
    static void create(std::vector<Carrot>& items, float x, float y);
    
    Carrot(float x, float y, float width = 1.2163742690058479f, float height = 0.9592720970537262f);
    
    static Carrot deserialize(rapidjson::Value& v);
};

#endif /* defined(__nosfuratu__Carrot__) */
