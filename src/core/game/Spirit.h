//
//  Spirit.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/9/16.
//  Copyright © 2016 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__Spirit__
#define __nosfuratu__Spirit__

#include "PhysicalEntity.h"

typedef enum
{
    SpiritType_Snake
} SpiritType;

class Spirit : public PhysicalEntity
{
public:
    static Spirit* create(float x, float y, SpiritType type);
    
    Spirit(float x, float y, float width, float height, SpiritType type);
    
    virtual void update(float deltaTime);
    
    SpiritType getType();
    
private:
    SpiritType m_type;
};

class SpiritSnake : public Spirit
{
public:
    SpiritSnake(float x, float y, float width = 1.828125f, float height = 2.25f, SpiritType type = SpiritType_Snake) : Spirit(x, y, width, height, type) {}
};

#endif /* defined(__nosfuratu__Spirit__) */
