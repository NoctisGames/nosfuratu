//
//  GameScreenStates.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__GameScreenStates__
#define __nosfuratu__GameScreenStates__

#include "State.h"

class GameScreen;

class GamePlay : public State<GameScreen>
{
public:
    static GamePlay* getInstance();
    
    virtual void enter(GameScreen* gs);
    
    virtual void execute(GameScreen* gs);
    
    virtual void exit(GameScreen* gs);
    
private:
    void handleTouchInput(GameScreen* gs);
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    GamePlay(){};
    GamePlay(const GamePlay&);
    GamePlay& operator=(const GamePlay&);
};

#endif /* defined(__nosfuratu__GameScreenStates__) */
