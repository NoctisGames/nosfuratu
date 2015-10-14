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
    JumpSpringGrass(float x, float y, float width = 1.0058479532163742f, float height = 1.0060658578856152f) : JumpSpring(x, y, width, height, JumpSpringType_Grass) {}
};

class JumpSpringCave : public JumpSpring
{
public:
    JumpSpringCave(float x, float y, float width = 1.2163742690058479f, float height = 1.5441941074523398f) : JumpSpring(x, y, width, height, JumpSpringType_Cave) {}
};

#endif /* defined(__nosfuratu__JumpSpring__) */
