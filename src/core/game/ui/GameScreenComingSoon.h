//
//  GameScreenComingSoon.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/6/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__GameScreenComingSoon__
#define __nosfuratu__GameScreenComingSoon__

#include "GameScreen.h"
#include "RTTI.h"

class ComingSoon : public GameScreenState
{
    RTTI_DECL;
    
public:
    static ComingSoon* getInstance();
    
    virtual void enter(GameScreen* gs);
    
    virtual void execute(GameScreen* gs);
    
    virtual void exit(GameScreen* gs);
    
    virtual void initRenderer(GameScreen* gs);
    
private:
    bool m_isRequestingNextState;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    ComingSoon();
    ComingSoon(const ComingSoon&);
    ComingSoon& operator=(const ComingSoon&);
};

#endif /* defined(__nosfuratu__GameScreenComingSoon__) */
