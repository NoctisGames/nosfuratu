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
#include "BackButton.h"

#include <memory>

class GameScreen;
class Game;

class Title : public State<GameScreen>
{
public:
    static Title* getInstance();
    
    virtual void enter(GameScreen* gs);
    
    virtual void execute(GameScreen* gs);
    
    virtual void exit(GameScreen* gs);
    
private:
    bool m_isRequestingNextState;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    Title();
    Title(const Title&);
    Title& operator=(const Title&);
};

class WorldMap : public State<GameScreen>
{
public:
    static WorldMap* getInstance();
    
    virtual void enter(GameScreen* gs);
    
    virtual void execute(GameScreen* gs);
    
    virtual void exit(GameScreen* gs);
    
private:
    std::unique_ptr<BackButton> m_backButton;
    int m_iLevelToLoad;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    WorldMap();
    WorldMap(const WorldMap&);
    WorldMap& operator=(const WorldMap&);
};

class GamePlay : public State<GameScreen>
{
public:
    static GamePlay* getInstance();
    
    virtual void enter(GameScreen* gs);
    
    virtual void execute(GameScreen* gs);
    
    virtual void exit(GameScreen* gs);
    
    void setSourceGame(Game* game);
    
protected:
    const char* m_json;
    std::unique_ptr<Game> m_game;
    Game* m_sourceGame;
    std::unique_ptr<BackButton> m_backButton;
    float m_fStateTime;
    bool m_isReleasingShockwave;
    float m_fShockwaveCenterX;
    float m_fShockwaveCenterY;
    float m_fShockwaveElapsedTime;
    bool m_hasShownOpeningSequence;
    bool m_hasOpeningSequenceCompleted;
    bool m_hasSwiped;
    bool m_showDeathTransOut;
    
    bool handleOpeningSequenceTouchInput(GameScreen* gs);
    bool handleTouchInput(GameScreen* gs);
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    GamePlay(const char* m_json);
    GamePlay(const GamePlay&);
    GamePlay& operator=(const GamePlay&);
};

#endif /* defined(__nosfuratu__GameScreenStates__) */
