//
//  JumpSpring.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__JumpSpring__
#define __nosfuratu__JumpSpring__

#include "LandPhysicalEntity.h"

typedef enum
{
    JumpSpringType_Light,
    JumpSpringType_Medium,
    JumpSpringType_Heavy
} JumpSpringType;

class JumpSpring : public LandPhysicalEntity
{
public:
    static JumpSpring* create(float x, float y, int type);
    
    JumpSpring(float x, float y, float width, float height, int boostVelocity, JumpSpringType type);
    
    void trigger();
    
    int getBoostVelocity();
    
    bool isBoosting();
    
    JumpSpringType getEnumType();
    
    int getType();
    
    virtual void updateBounds() = 0;
    
    virtual void update(float deltaTime) = 0;
    
protected:
    int m_iBoostVelocity;
    bool m_isTriggered;
    bool m_isBoosting;
    
private:
    JumpSpringType m_type;
};

class JumpSpringLight : public JumpSpring
{
public:
    JumpSpringLight(float x, float y, float width = 1.4382022471910112f, float height = 1.018641810918775f) : JumpSpring(x, y, width, height, 18, JumpSpringType_Light) {}
    
    virtual void updateBounds();
    
    virtual void update(float deltaTime);
};

class JumpSpringMedium : public JumpSpring
{
public:
    JumpSpringMedium(float x, float y, float width = 3.295880149812734f, float height = 1.689747003994674f) : JumpSpring(x, y, width, height, 27, JumpSpringType_Medium) {}
    
    virtual void updateBounds();
    
    virtual void update(float deltaTime);
};

class JumpSpringHeavy : public JumpSpring
{
public:
    JumpSpringHeavy(float x, float y, float width = 3.1760299625468167f, float height = 2.6245006657789616f) : JumpSpring(x, y, width, height, 36, JumpSpringType_Heavy) {}
    
    virtual void updateBounds();
    
    virtual void update(float deltaTime);
};

#endif /* defined(__nosfuratu__JumpSpring__) */
