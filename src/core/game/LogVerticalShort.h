//
//  LogVerticalShort.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__LogVerticalShort__
#define __nosfuratu__LogVerticalShort__

#include "DestructiblePhysicalEntity.h"

class LogVerticalShort : public DestructiblePhysicalEntity
{
public:
    static LogVerticalShort* create(float x, float y, int type);
    
    LogVerticalShort(float x, float y, float width = 3.655430711610487f, float height = 1.3422103861517976f);
    
    virtual void update(float deltaTime);
    
    virtual void updateBounds();
    
    virtual void triggerHit();
    
    int getType();
    
    virtual GroundSoundType getGroundSoundType();
    
private:
    bool m_isBlowingUp;
};

#endif /* defined(__nosfuratu__LogVerticalShort__) */
