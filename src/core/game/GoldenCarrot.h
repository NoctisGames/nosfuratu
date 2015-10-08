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

#include <vector>

class GoldenCarrot : public PhysicalEntity
{
public:
    static void create(std::vector<GoldenCarrot>& items, float x, float y);
    
    GoldenCarrot(float x, float y, float width = 1.21637426900585f, float height = 1.14644714038128f);
    
    static GoldenCarrot deserialize(rapidjson::Value& v);
};

#endif /* defined(__nosfuratu__GoldenCarrot__) */
