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

class LogVerticalTall : public DestructiblePhysicalEntity
{
public:
    static LogVerticalTall* create(int gridX, int gridY, int type);
    
    LogVerticalTall(float x, float y, float width = 3.0921348314606742f, float height = 3.0679094540612515f);
    
    virtual void update(float deltaTime);
    
    virtual void updateBounds();
    
    virtual void triggerHit();
    
    int getType();
    
    virtual GroundSoundType getGroundSoundType();
    
private:
    bool m_isBlowingUp;
};

#endif /* defined(__nosfuratu__LogVerticalTall__) */
