//
//  MainScreenTransitions.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/10/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__MainScreenTransitions__
#define __nosfuratu__MainScreenTransitions__

#include "MainScreenState.h"

#include "RTTI.h"

class MainScreen;
class Level;
class Game;

class TitleToDemo : public MainScreenState
{
    RTTI_DECL;
    
public:
    static TitleToDemo* getInstance();
    
    virtual void enter(MainScreen* gs);
    
    virtual void execute(MainScreen* gs);
    
    virtual void exit(MainScreen* gs);
    
    virtual void initRenderer(MainScreen* gs);
    
    void setWorldToLoad(int worldToLoad);
    
    void setLevelToLoad(int levelToLoad);
    
private:
    Level* m_levelState;
    float m_fTransitionStateTime;
    int m_iWorldToLoad;
    int m_iLevelToLoad;
    float m_fWaitTime;
    bool m_hasStoppedMusic;
    bool m_hasLoadedNextScreen;
    bool m_hasEnteredNextScreen;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    TitleToDemo();
    TitleToDemo(const TitleToDemo&);
    TitleToDemo& operator=(const TitleToDemo&);
};

class TitleToWorldMap : public MainScreenState
{
    RTTI_DECL;
    
public:
    static TitleToWorldMap* getInstance();
    
    virtual void enter(MainScreen* gs);
    
    virtual void execute(MainScreen* gs);
    
    virtual void exit(MainScreen* gs);
    
    virtual void initRenderer(MainScreen* gs);
    
private:
    float m_fTransitionStateTime;
    float m_fWaitTime;
    bool m_hasStoppedMusic;
    bool m_hasLoadedNextScreen;
    bool m_hasEnteredNextScreen;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    TitleToWorldMap();
    TitleToWorldMap(const TitleToWorldMap&);
    TitleToWorldMap& operator=(const TitleToWorldMap&);
};

class TitleToOpeningCutscene : public MainScreenState
{
    RTTI_DECL;
    
public:
    static TitleToOpeningCutscene* getInstance();
    
    virtual void enter(MainScreen* gs);
    
    virtual void execute(MainScreen* gs);
    
    virtual void exit(MainScreen* gs);
    
    virtual void initRenderer(MainScreen* gs);
    
private:
    float m_fTransitionStateTime;
    float m_fWaitTime;
    bool m_hasStoppedMusic;
    bool m_hasLoadedNextScreen;
    bool m_hasEnteredNextScreen;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    TitleToOpeningCutscene();
    TitleToOpeningCutscene(const TitleToOpeningCutscene&);
    TitleToOpeningCutscene& operator=(const TitleToOpeningCutscene&);
};

class OpeningCutsceneToWorldMap : public MainScreenState
{
    RTTI_DECL;
    
public:
    static OpeningCutsceneToWorldMap* getInstance();
    
    virtual void enter(MainScreen* gs);
    
    virtual void execute(MainScreen* gs);
    
    virtual void exit(MainScreen* gs);
    
    virtual void initRenderer(MainScreen* gs);
    
private:
    float m_fTransitionStateTime;
    float m_fWaitTime;
    bool m_hasStoppedMusic;
    bool m_hasLoadedNextScreen;
    bool m_hasEnteredNextScreen;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    OpeningCutsceneToWorldMap();
    OpeningCutsceneToWorldMap(const OpeningCutsceneToWorldMap&);
    OpeningCutsceneToWorldMap& operator=(const OpeningCutsceneToWorldMap&);
};

class TitleToLevelEditor : public MainScreenState
{
    RTTI_DECL;
    
public:
    static TitleToLevelEditor* getInstance();
    
    virtual void enter(MainScreen* gs);
    
    virtual void execute(MainScreen* gs);
    
    virtual void exit(MainScreen* gs);
    
    virtual void initRenderer(MainScreen* gs);
    
private:
    float m_fTransitionStateTime;
    float m_fWaitTime;
    bool m_hasStoppedMusic;
    bool m_hasLoadedNextScreen;
    bool m_hasEnteredNextScreen;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    TitleToLevelEditor();
    TitleToLevelEditor(const TitleToLevelEditor&);
    TitleToLevelEditor& operator=(const TitleToLevelEditor&);
};

class WorldMapToOpeningCutscene : public MainScreenState
{
    RTTI_DECL;
    
public:
    static WorldMapToOpeningCutscene* getInstance();
    
    virtual void enter(MainScreen* gs);
    
    virtual void execute(MainScreen* gs);
    
    virtual void exit(MainScreen* gs);
    
    virtual void initRenderer(MainScreen* gs);
    
    void setCutsceneButtonLocation(float centerX, float centerY);
    
private:
    float m_fFade;
    float m_fCenterX;
    float m_fCenterY;
    float m_fTransitionStateTime;
    float m_fWaitTime;
    bool m_hasStoppedMusic;
    bool m_hasPlayedTransitionSound;
    bool m_hasEnteredNextScreen;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    WorldMapToOpeningCutscene();
    WorldMapToOpeningCutscene(const WorldMapToOpeningCutscene&);
    WorldMapToOpeningCutscene& operator=(const WorldMapToOpeningCutscene&);
};

class WorldMapToLevel : public MainScreenState
{
    RTTI_DECL;
    
public:
    static WorldMapToLevel* getInstance();
    
    virtual void enter(MainScreen* gs);
    
    virtual void execute(MainScreen* gs);
    
    virtual void exit(MainScreen* gs);
    
    virtual void initRenderer(MainScreen* gs);
    
    void setLevelLocation(float centerX, float centerY);
    
    void setWorldToLoad(int worldToLoad);
    
    void setLevelToLoad(int levelToLoad);
    
    void setBestStats(int bestScore, int bestLevelStatsFlag, int numGoldenCarrots, int jonAbilityFlag);
    
private:
    Level* m_levelState;
    float m_fCenterX;
    float m_fCenterY;
    float m_fTransitionStateTime;
    float m_fWaitTime;
    float m_fFade;
    int m_iWorldToLoad;
    int m_iLevelToLoad;
    bool m_hasStoppedMusic;
    bool m_hasPlayedTransitionSound;
    bool m_hasEnteredLevel;
    
    // Set from app storage
    int m_iBestScore;
    int m_iBestLevelStatsFlag;
    int m_iLastKnownNumGoldenCarrots;
    int m_iLastKnownJonAbilityFlag;
    bool m_needsToPlayTransitionSound;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    WorldMapToLevel();
    WorldMapToLevel(const WorldMapToLevel&);
    WorldMapToLevel& operator=(const WorldMapToLevel&);
};

class LevelToComingSoon : public MainScreenState
{
    RTTI_DECL;
    
public:
    static LevelToComingSoon* getInstance();
    
    virtual void enter(MainScreen* gs);
    
    virtual void execute(MainScreen* gs);
    
    virtual void exit(MainScreen* gs);
    
    virtual void initRenderer(MainScreen* gs);
    
    void setLevelComingFrom(Level* levelState);
    
private:
    Level* m_levelState;
    float m_fTransitionStateTime;
    float m_fWaitTime;
    bool m_hasStoppedMusic;
    bool m_hasLoadedNextScreen;
    bool m_hasEnteredNextScreen;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    LevelToComingSoon();
    LevelToComingSoon(const LevelToComingSoon&);
    LevelToComingSoon& operator=(const LevelToComingSoon&);
};

#endif /* defined(__nosfuratu__MainScreenTransitions__) */
