//
//  GameScreenOpeningCutscene.h
//  nosfuratu
//
//  Created by Stephen Gowen on 8/2/16.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__GameScreenOpeningCutscene__
#define __nosfuratu__GameScreenOpeningCutscene__

#include "State.h"

#include <memory>

class GameScreen;

class OpeningCutscene : public State<GameScreen>
{
public:
    static OpeningCutscene* getInstance();
    
    virtual void enter(GameScreen* gs);
    
    virtual void execute(GameScreen* gs);
    
    virtual void exit(GameScreen* gs);
    
private:
    float m_fStateTime;
    float m_fSkipTime;
    bool m_isRequestingNextState;
    bool m_isSkipping;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    OpeningCutscene();
    OpeningCutscene(const OpeningCutscene&);
    OpeningCutscene& operator=(const OpeningCutscene&);
};

#endif /* defined(__nosfuratu__GameScreenOpeningCutscene__) */
