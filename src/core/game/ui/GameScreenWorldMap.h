//
//  GameScreenWorldMap.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/10/16.
//  Copyright © 2016 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__GameScreenWorldMap__
#define __nosfuratu__GameScreenWorldMap__

#include "State.h"
#include "BackButton.h"
#include "GameScreenLevels.h"

#include <memory>

class GameScreen;

class WorldMap : public State<GameScreen>
{
public:
    static WorldMap* getInstance();
    
    virtual void enter(GameScreen* gs);
    
    virtual void execute(GameScreen* gs);
    
    virtual void exit(GameScreen* gs);
    
    BackButton& getBackButton();
    
private:
    std::unique_ptr<BackButton> m_backButton;
    int m_iLevelToLoad;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    WorldMap();
    WorldMap(const WorldMap&);
    WorldMap& operator=(const WorldMap&);
};

#endif /* defined(__nosfuratu__GameScreenWorldMap__) */
