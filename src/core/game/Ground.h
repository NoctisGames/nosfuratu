//
//  Ground.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/5/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__Ground__
#define __nosfuratu__Ground__

#include "PhysicalEntity.h"

class Ground : public PhysicalEntity
{
public:
    Ground(float x, float y, float width, float height, int groundType);
    
    int getGroundType();
    
private:
    int m_iGroundType;
};

#endif /* defined(__nosfuratu__Ground__) */
