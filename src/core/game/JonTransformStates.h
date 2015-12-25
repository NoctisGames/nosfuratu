//
//  JonTransformStates.h
//  nosfuratu
//
//  Created by Stephen Gowen on 12/25/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__JonTransformStates__
#define __nosfuratu__JonTransformStates__

#include "JonFormState.h"

class RabbitToVampire : public JonFormState
{
public:
    static RabbitToVampire* getInstance();
    
    virtual void enter(Jon* jon);
    virtual void execute(Jon* jon);
    virtual void exit(Jon* jon);
    
    virtual void triggerTransform(Jon* jon) {};
    
    virtual void triggerJump(Jon* jon) {};
    virtual void triggerLeftAction(Jon* jon) {};
    virtual void triggerRightAction(Jon* jon) {};
    virtual void triggerUpAction(Jon* jon) {};
    virtual void triggerDownAction(Jon* jon) {};
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    RabbitToVampire() {};
    RabbitToVampire(const RabbitToVampire&);
    RabbitToVampire& operator=(const RabbitToVampire&);
};

class VampireToRabbit : public JonFormState
{
public:
    static VampireToRabbit* getInstance();
    
    virtual void enter(Jon* jon);
    virtual void execute(Jon* jon);
    virtual void exit(Jon* jon);
    
    virtual void triggerTransform(Jon* jon) {};
    
    virtual void triggerJump(Jon* jon) {};
    virtual void triggerLeftAction(Jon* jon) {};
    virtual void triggerRightAction(Jon* jon) {};
    virtual void triggerUpAction(Jon* jon) {};
    virtual void triggerDownAction(Jon* jon) {};
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    VampireToRabbit() {};
    VampireToRabbit(const VampireToRabbit&);
    VampireToRabbit& operator=(const VampireToRabbit&);
};

#endif /* defined(__nosfuratu__JonTransformStates__) */
