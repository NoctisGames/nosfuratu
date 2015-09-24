//
//  EntityUtils.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/23/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__EntityUtils__
#define __nosfuratu__EntityUtils__

#include "EntityAnchor.h"

class PhysicalEntity;

class EntityUtils
{
public:
    static void applyAnchor(PhysicalEntity& entity, EntityAnchor anchor);
    
    static void attach(PhysicalEntity& entity, PhysicalEntity& to, bool leftOf);
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    EntityUtils();
    EntityUtils(const EntityUtils&);
    EntityUtils& operator=(const EntityUtils&);
};

#endif /* defined(__nosfuratu__EntityUtils__) */