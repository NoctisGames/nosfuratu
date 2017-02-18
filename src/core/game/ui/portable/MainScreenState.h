//
//  MainScreenState.h
//  nosfuratu
//
//  Created by Stephen Gowen on 11/5/16.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#ifndef MainScreenState_h
#define MainScreenState_h

#include "State.h"
#include "RTTI.h"

class MainScreen;

class MainScreenState : public State<MainScreen>
{
    RTTI_DECL;
    
public:
    virtual void enter(MainScreen* gs) = 0;
    
    virtual void execute(MainScreen* gs) = 0;
    
    virtual void exit(MainScreen* gs) = 0;
    
    MainScreenState() {}
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    MainScreenState(const MainScreenState&);
    MainScreenState& operator=(const MainScreenState&);
};

#endif /* MainScreenState_h */
