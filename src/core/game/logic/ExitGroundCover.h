//
//  ExitGroundCover.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/9/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__ExitGroundCover__
#define __nosfuratu__ExitGroundCover__

#include "PhysicalEntity.h"

#include "ExitGroundCoverType.h"

#include "RTTI.h"

class ExitGroundCover : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    ExitGroundCover(float x, float y, float width, float height, ExitGroundCoverType type);
    
    virtual void update(float deltaTime);
    
    void triggerHit();
    
    ExitGroundCoverType getType();
    
private:
    ExitGroundCoverType m_type;
    bool m_isBreaking;
};

#endif /* defined(__nosfuratu__ExitGroundCover__) */
