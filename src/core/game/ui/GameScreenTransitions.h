//
//  GameScreenTransitions.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/10/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
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
    
    void setLevelLocation(float centerX, float centerY);
    
    void setWorldToLoad(int worldToLoad);
    
    void setLevelToLoad(int levelToLoad);
    
    void setBestStats(int bestScore, int bestOnlineScore, int bestLevelStatsFlag, int numGoldenCarrots, int jonAbilityFlag);
    
private:
    Level* m_levelState;
    float m_fCenterX;
    float m_fCenterY;
    float m_fTransitionStateTime;
    int m_iWorldToLoad;
    int m_iLevelToLoad;
    
    // Set from app storage
    int m_iBestScore;
    int m_iBestOnlineScore;
    int m_iBestLevelStatsFlag;
    int m_iLastKnownNumGoldenCarrots;
    int m_iLastKnownJonAbilityFlag;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    WorldMapToLevel();
    WorldMapToLevel(const WorldMapToLevel&);
    WorldMapToLevel& operator=(const WorldMapToLevel&);
};

#endif /* defined(__nosfuratu__GameScreenTransitions__) */
