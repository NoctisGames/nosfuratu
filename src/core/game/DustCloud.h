//
//  DustCloud.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__DustCloud__
#define __nosfuratu__DustCloud__

#include "PhysicalEntity.h"
#include "Color.h"

class DustCloud : public PhysicalEntity
{
public:
    DustCloud(float x, float y, float scale);
    
    virtual void update(float deltaTime);
    
    Color getColor();
    
private:
    Color m_color;
};

#endif /* defined(__nosfuratu__DustCloud__) */
