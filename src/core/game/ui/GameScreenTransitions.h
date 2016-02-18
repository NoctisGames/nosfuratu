//
//  GameScreenTransitions.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/10/16.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__GameScreenTransitions__
#define __nosfuratu__GameScreenTransitions__

#include "State.h"
#include "GameScreenLevels.h"

#include <memory>

class GameScreen;
class Game;

class TitleToWorldMap : public State<GameScreen>
{
public:
    static TitleToWorldMap* getInstance();
    
    virtual void enter(GameScreen* gs);
    
    virtual void execute(GameScreen* gs);
    
    virtual void exit(GameScreen* gs);
    
private:
    float m_fTransitionStateTime;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    TitleToWorldMap();
    TitleToWorldMap(const TitleToWorldMap&);
    TitleToWorldMap& operator=(const TitleToWorldMap&);
};

class TitleToLevelEditor : public State<GameScreen>
{
public:
    static TitleToLevelEditor* getInstance();
    
    virtual void enter(GameScreen* gs);
    
    virtual void execute(GameScreen* gs);
    
    virtual void exit(GameScreen* gs);
    
private:
    float m_fTransitionStateTime;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    TitleToLevelEditor();
    TitleToLevelEditor(const TitleToLevelEditor&);
    TitleToLevelEditor& operator=(const TitleToLevelEditor&);
};

class WorldMapToLevel : public State<GameScreen>
{
public:
    static WorldMapToLevel* getInstance();
    
    virtual void enter(GameScreen* gs);
    
    virtual void execute(GameScreen* gs);
    
    virtual void exit(GameScreen* gs);
    
    void setLevelToLoad(int levelToLoad);
    
private:
    Level* m_levelState;
    float m_fTransitionStateTime;
    int m_iLevelToLoad;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    WorldMapToLevel();
    WorldMapToLevel(const WorldMapToLevel&);
    WorldMapToLevel& operator=(const WorldMapToLevel&);
};

#endif /* defined(__nosfuratu__GameScreenTransitions__) */
