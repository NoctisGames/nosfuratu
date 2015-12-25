//
//  JonFormState.h
//  nosfuratu
//
//  Created by Stephen Gowen on 12/25/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__JonFormState__
#define __nosfuratu__JonFormState__

#include "State.h"

class Jon;

class JonFormState : public State<Jon>
{
public:
    virtual void enter(Jon* jon) = 0;
    virtual void execute(Jon* jon) = 0;
    virtual void exit(Jon* jon) = 0;
    
    virtual void triggerTransform(Jon* jon) = 0;
    
    virtual void triggerJump(Jon* jon) = 0;
    virtual void triggerLeftAction(Jon* jon) = 0;
    virtual void triggerRightAction(Jon* jon) = 0;
    virtual void triggerUpAction(Jon* jon) = 0;
    virtual void triggerDownAction(Jon* jon) = 0;
    
    JonFormState() {};
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    JonFormState(const JonFormState&);
    JonFormState& operator=(const JonFormState&);
};

#endif /* defined(__nosfuratu__JonFormState__) */
