//
//  CaveExitCover.h
//  nosfuratu
//
//  Created by Stephen Gowen on 11/2/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__CaveExitCover__
#define __nosfuratu__CaveExitCover__

#include "DestructiblePhysicalEntity.h"
#include "Color.h"

class CaveExitCover : public DestructiblePhysicalEntity
{
public:
    CaveExitCover(float x, float y = 8.095320623916809f, float width = 3.2046783625730995f, float height = 2.246100519930676f);
    
    virtual void update(float deltaTime);
    
    virtual void triggerHit();
    
private:
    Color m_color;
    bool m_isBreaking;
};

#endif /* defined(__nosfuratu__CaveExitCover__) */
