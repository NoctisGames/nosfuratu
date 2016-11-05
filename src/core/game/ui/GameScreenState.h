//
//  GameScreenState.h
//  nosfuratu
//
//  Created by Stephen Gowen on 11/5/16.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#ifndef GameScreenState_h
#define GameScreenState_h

#include "State.h"
#include "RTTI.h"

class GameScreen;

class GameScreenState : public State<GameScreen>
{
    RTTI_DECL;
    
public:
    virtual void enter(GameScreen* gs) = 0;
    
    virtual void execute(GameScreen* gs) = 0;
    
    virtual void exit(GameScreen* gs) = 0;
    
    virtual void initRenderer(GameScreen* gs) = 0;
    
    GameScreenState() {}
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    GameScreenState(const GameScreenState&);
    GameScreenState& operator=(const GameScreenState&);
};

#endif /* GameScreenState_h */
