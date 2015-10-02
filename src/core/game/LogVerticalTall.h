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
#include "EntityAnchor.h"

#include <vector>

class LogVerticalTall : public DestructiblePhysicalEntity
{
public:
    static void create(std::vector<LogVerticalTall>& items, float x, EntityAnchor anchor = EntityAnchor::ANCHOR_GROUND);
    
    LogVerticalTall(float x, float y, float width = 1.6608187134502923f, float height = 2.1291161178509532f);
    
    virtual void updateBounds();
    
    static LogVerticalTall deserialize(rapidjson::Value& v);
};

#endif /* defined(__nosfuratu__LogVerticalTall__) */
