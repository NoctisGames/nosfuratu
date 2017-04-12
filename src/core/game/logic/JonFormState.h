//
//  JonFormState.h
//  nosfuratu
//
//  Created by Stephen Gowen on 11/4/16.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#ifndef JonFormState_h
#define JonFormState_h

#include "State.h"

#include "RTTI.h"

class Jon;

class JonFormState : public State<Jon>
{
    RTTI_DECL;
    
public:
    virtual void enter(Jon* jon) = 0;
    virtual void execute(Jon* jon) = 0;
    virtual void exit(Jon* jon) = 0;
    
    virtual void triggerTransform(Jon* jon) = 0;
    virtual void triggerCancelTransform(Jon* jon) = 0;
    
    virtual void triggerJump(Jon* jon) = 0;
    virtual void triggerLeftAction(Jon* jon) = 0;
    virtual void triggerRightAction(Jon* jon) = 0;
    virtual void triggerUpAction(Jon* jon) = 0;
    virtual void triggerDownAction(Jon* jon) = 0;
    
    virtual void triggerBoost(Jon* jon, float boostVelocity) = 0;
    virtual void triggerBoostOffEnemy(Jon* jon, float boostVelocity) = 0;
    virtual void triggerBounceDownardsOffEnemy(Jon* jon, float bounceBackVelocity) = 0;
    virtual void triggerBounceBackOffEnemy(Jon* jon, float bounceBackVelocity) = 0;
    
    virtual void onDeath(Jon* jon) = 0;
    
    virtual int getNumJumps(Jon* jon) = 0;
    
    JonFormState();
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    JonFormState(const JonFormState&);
    JonFormState& operator=(const JonFormState&);
};

#endif /* JonFormState_h */
