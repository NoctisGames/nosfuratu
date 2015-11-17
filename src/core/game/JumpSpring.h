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
    JumpSpringType_Grass,
    JumpSpringType_Cave
} JumpSpringType;

class JumpSpring : public PhysicalEntity
{
public:
    static JumpSpring* create(float x, float y, int type);
    
    JumpSpring(float x, float y, float width, float height, JumpSpringType type);
    
    virtual void updateBounds();
    
    virtual void update(float deltaTime);
    
    void trigger();
    
    bool isTriggered();
    
    JumpSpringType getEnumType();
    
    int getType();
    
private:
    JumpSpringType m_type;
    bool m_isTriggered;
};

class JumpSpringGrass : public JumpSpring
{
public:
    JumpSpringGrass(float x, float y, float width = 0.9827715355805243f, float height = 0.9707057256990679f) : JumpSpring(x, y, width, height, JumpSpringType_Grass) {}
};

class JumpSpringCave : public JumpSpring
{
public:
    JumpSpringCave(float x, float y, float width = 0.6232209737827715f, float height = 0.7909454061251664f) : JumpSpring(x, y, width, height, JumpSpringType_Cave) {}
};

#endif /* defined(__nosfuratu__JumpSpring__) */
