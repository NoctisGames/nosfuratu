//
//  JumpSpring.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__JumpSpring__
#define __nosfuratu__JumpSpring__

#include "PhysicalEntity.h"

typedef enum
{
    JumpSpringType_Light,
    JumpSpringType_Medium,
    JumpSpringType_Heavy
} JumpSpringType;

class JumpSpring : public PhysicalEntity
{
public:
    static JumpSpring* create(float x, float y, int type);
    
    JumpSpring(float x, float y, float width, float height, JumpSpringType type);
    
    virtual void updateBounds();
    
    virtual void update(float deltaTime);
    
    void trigger();
    
    bool isBoosting();
    
    JumpSpringType getEnumType();
    
    int getType();
    
private:
    JumpSpringType m_type;
    bool m_isTriggered;
    bool m_isBoosting;
};

class JumpSpringLight : public JumpSpring
{
public:
    JumpSpringLight(float x, float y, float width = 1.4382022471910112f, float height = 1.018641810918775f) : JumpSpring(x, y, width, height, JumpSpringType_Light) {}
};

class JumpSpringMedium : public JumpSpring
{
public:
    JumpSpringMedium(float x, float y, float width = 1.4382022471910112f, float height = 1.018641810918775f) : JumpSpring(x, y, width, height, JumpSpringType_Medium) {}
};

class JumpSpringHeavy : public JumpSpring
{
public:
    JumpSpringHeavy(float x, float y, float width = 1.4382022471910112f, float height = 1.018641810918775f) : JumpSpring(x, y, width, height, JumpSpringType_Heavy) {}
};

#endif /* defined(__nosfuratu__JumpSpring__) */
