//
//  MainScreenComingSoon.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/6/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__MainScreenComingSoon__
#define __nosfuratu__MainScreenComingSoon__

#include "MainScreen.h"
#include "RTTI.h"

class ComingSoon : public MainScreenState
{
    RTTI_DECL;
    
public:
    static ComingSoon* getInstance();
    
    virtual void enter(MainScreen* gs);
    
    virtual void execute(MainScreen* gs);
    
    virtual void exit(MainScreen* gs);
    
private:
    bool m_isRequestingNextState;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    ComingSoon();
    ComingSoon(const ComingSoon&);
    ComingSoon& operator=(const ComingSoon&);
};

#endif /* defined(__nosfuratu__MainScreenComingSoon__) */
