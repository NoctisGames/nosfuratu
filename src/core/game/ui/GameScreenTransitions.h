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
#include "GameScreenState.h"
#include "RTTI.h"

#include <memory>

class GameScreen;
class Game;

class TitleToWorldMap : public GameScreenState
{
    RTTI_DECL;
    
public:
    static TitleToWorldMap* getInstance();
    
    virtual void enter(GameScreen* gs);
    
    virtual void execute(GameScreen* gs);
    
    virtual void exit(GameScreen* gs);
    
    virtual void initRenderer(GameScreen* gs) {}
    
private:
    float m_fTransitionStateTime;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    TitleToWorldMap();
    TitleToWorldMap(const TitleToWorldMap&);
    TitleToWorldMap& operator=(const TitleToWorldMap&);
};

class TitleToOpeningCutscene : public GameScreenState
{
    RTTI_DECL;
    
public:
    static TitleToOpeningCutscene* getInstance();
    
    virtual void enter(GameScreen* gs);
    
    virtual void execute(GameScreen* gs);
    
    virtual void exit(GameScreen* gs);
    
    virtual void initRenderer(GameScreen* gs) {}
    
private:
    float m_fTransitionStateTime;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    TitleToOpeningCutscene();
    TitleToOpeningCutscene(const TitleToOpeningCutscene&);
    TitleToOpeningCutscene& operator=(const TitleToOpeningCutscene&);
};

class OpeningCutsceneToWorldMap : public GameScreenState
{
    RTTI_DECL;
    
public:
    static OpeningCutsceneToWorldMap* getInstance();
    
    virtual void enter(GameScreen* gs);
    
    virtual void execute(GameScreen* gs);
    
    virtual void exit(GameScreen* gs);
    
    virtual void initRenderer(GameScreen* gs) {}
    
private:
    float m_fTransitionStateTime;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    OpeningCutsceneToWorldMap();
    OpeningCutsceneToWorldMap(const OpeningCutsceneToWorldMap&);
    OpeningCutsceneToWorldMap& operator=(const OpeningCutsceneToWorldMap&);
};

class TitleToLevelEditor : public GameScreenState
{
    RTTI_DECL;
    
public:
    static TitleToLevelEditor* getInstance();
    
    virtual void enter(GameScreen* gs);
    
    virtual void execute(GameScreen* gs);
    
    virtual void exit(GameScreen* gs);
    
    virtual void initRenderer(GameScreen* gs) {}
    
private:
    float m_fTransitionStateTime;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    TitleToLevelEditor();
    TitleToLevelEditor(const TitleToLevelEditor&);
    TitleToLevelEditor& operator=(const TitleToLevelEditor&);
};

class WorldMapToOpeningCutscene : public GameScreenState
{
    RTTI_DECL;
    
public:
    static WorldMapToOpeningCutscene* getInstance();
    
    virtual void enter(GameScreen* gs);
    
    virtual void execute(GameScreen* gs);
    
    virtual void exit(GameScreen* gs);
    
    virtual void initRenderer(GameScreen* gs) {}
    
    void setCutsceneButtonLocation(float centerX, float centerY);
    
private:
    float m_fFade;
    float m_fCenterX;
    float m_fCenterY;
    float m_fTransitionStateTime;
    bool m_hasPlayedTransitionSound;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    WorldMapToOpeningCutscene();
    WorldMapToOpeningCutscene(const WorldMapToOpeningCutscene&);
    WorldMapToOpeningCutscene& operator=(const WorldMapToOpeningCutscene&);
};

class WorldMapToLevel : public GameScreenState
{
    RTTI_DECL;
    
public:
    static WorldMapToLevel* getInstance();
    
    virtual void enter(GameScreen* gs);
    
    virtual void execute(GameScreen* gs);
    
    virtual void exit(GameScreen* gs);
    
    virtual void initRenderer(GameScreen* gs) {}
    
    void setLevelLocation(float centerX, float centerY);
    
    void setWorldToLoad(int worldToLoad);
    
    void setLevelToLoad(int levelToLoad);
    
    void setBestStats(int bestScore, int bestOnlineScore, int bestLevelStatsFlag, int numGoldenCarrots, int jonAbilityFlag);
    
private:
    Level* m_levelState;
    float m_fCenterX;
    float m_fCenterY;
    float m_fTransitionStateTime;
    float m_fFade;
    int m_iWorldToLoad;
    int m_iLevelToLoad;
    bool m_hasPlayedTransitionSound;
    
    // Set from app storage
    int m_iBestScore;
    int m_iBestOnlineScore;
    int m_iBestLevelStatsFlag;
    int m_iLastKnownNumGoldenCarrots;
    int m_iLastKnownJonAbilityFlag;
    bool m_needsToPlayTransitionSound;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    WorldMapToLevel();
    WorldMapToLevel(const WorldMapToLevel&);
    WorldMapToLevel& operator=(const WorldMapToLevel&);
};

class LevelToComingSoon : public GameScreenState
{
    RTTI_DECL;
    
public:
    static LevelToComingSoon* getInstance();
    
    virtual void enter(GameScreen* gs);
    
    virtual void execute(GameScreen* gs);
    
    virtual void exit(GameScreen* gs);
    
    virtual void initRenderer(GameScreen* gs) {}
    
    void setLevelComingFrom(Level* levelState);
    
private:
    Level* m_levelState;
    float m_fTransitionStateTime;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    LevelToComingSoon();
    LevelToComingSoon(const LevelToComingSoon&);
    LevelToComingSoon& operator=(const LevelToComingSoon&);
};

#endif /* defined(__nosfuratu__GameScreenTransitions__) */
