//
//  ExitGroundCover.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/9/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__ExitGroundCover__
#define __nosfuratu__ExitGroundCover__

#include "PhysicalEntity.h"
#include "Color.h"

typedef enum
{
    ExitGroundCoverType_Grass,
    ExitGroundCoverType_Cave
} ExitGroundCoverType;

class ExitGroundCover : public PhysicalEntity
{
public:
    ExitGroundCover(float x, float y, float width, float height, ExitGroundCoverType type);
    
    virtual void update(float deltaTime);
    
    void triggerHit();
    
    ExitGroundCoverType getType();
    
    Color getColor();
    
private:
    ExitGroundCoverType m_type;
    Color m_color;
    bool m_isBreaking;
};

#endif /* defined(__nosfuratu__ExitGroundCover__) */
