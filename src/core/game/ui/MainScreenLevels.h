//
//  MainScreenLevels.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/10/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__MainScreenLevels__
#define __nosfuratu__MainScreenLevels__

#include "MainScreenState.h"

#include "RTTI.h"

#include <vector>

class MainScreen;
class Game;
class GameButton;
class LevelCompletePanel;
class CollectibleItem;
class MidBossOwl;
class EndBossSnake;
class Hole;
class ForegroundObject;
class BatPanel;
class PhysicalEntity;

typedef enum
{
    DemoAction_Exit,
    DemoAction_TriggerJump,
    DemoAction_TriggerTransform,
    DemoAction_TriggerRight,
    DemoAction_TriggerUp,
    DemoAction_TriggerLeft,
    DemoAction_TriggerDown
} DemoAction;

class UserDemoAction
{
public:
    DemoAction m_action;
    float m_fStateTimeToExecuteAction;
    
    UserDemoAction(DemoAction action, float stateTimeToExecuteAction);
};

class Level : public MainScreenState
{
    RTTI_DECL;
    
public:
    virtual void enter(MainScreen* ms);
    
    virtual void execute(MainScreen* ms);
    
    virtual void exit(MainScreen* ms);
    
    virtual void initRenderer(MainScreen* ms);
    
    void setSourceGame(Game* game);
    
    void setBestStats(int bestScore, int bestLevelStatsFlag, int numGoldenCarrots, int jonAbilityFlag);
    
    void launchInDemoMode(std::vector<UserDemoAction> userDemoActions);
    
    bool hasCompletedLevel();
    
    Game& getGame();
    
    LevelCompletePanel* getLevelCompletePanel();
    
    bool isDebug();
    
    float getStateTime();
    
protected:
    const char* m_json;
    Game* m_game;
    Game* m_sourceGame;
    GameButton* m_backButton;
    LevelCompletePanel* m_levelCompletePanel;
    BatPanel* m_batPanel;
    float m_fStateTime;
    int m_iLevelStatsFlag;
    int m_iNumGoldenCarrots;
    int m_iNumTimesBatPanelDisplayed;
    int m_iNumAttemptsSinceLastAdBreak;
    bool m_hasShownOpeningSequence;
    bool m_hasOpeningSequenceCompleted;
    bool m_activateRadialBlur;
    bool m_showDeathTransOut;
    bool m_exitLoop;
    bool m_hasCompletedLevel;
    bool m_isDisplayingResults;
    bool m_createdOwnSourceGame; // Can also be injected by the Level Editor
    std::vector<UserDemoAction> m_userDemoActions;
    bool m_isDemoMode;
    bool m_isDebug;
    bool m_isShowingBounds;
    bool m_hasExited;
    
    // Set from app storage
    int m_iBestScore;
    int m_iBestLevelStatsFlag;
    int m_iLastKnownNumGoldenCarrots;
    int m_iLastKnownJonAbilityFlag;
    
    virtual void onEnter(MainScreen* ms);

	virtual void beginOpeningSequence(MainScreen* ms);

	virtual void handleOpeningSequence(MainScreen* ms);
    
    virtual void update(MainScreen* ms);
    
    virtual void render(MainScreen* ms);
    
    virtual void updateCamera(MainScreen* ms, float paddingX = 0, bool ignoreY = false, bool instant = false);
    
    virtual void additionalRenderingBeforeHud(MainScreen* ms);
    
    virtual bool isInSlowMotionMode();
    
    virtual void configBatPanel();
    
    bool handleInput(MainScreen* ms);
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    Level(const char* m_json = nullptr);
    virtual ~Level();
    Level(const Level&);
    Level& operator=(const Level&);
    
private:
    bool m_playLevelSelectMusicOnExit;
	bool m_stopMusicOnExit;
    
    void handleCollections(PhysicalEntity& entity, std::vector<CollectibleItem *>& items, float deltaTime);
};

class Chapter1Level1 : public Level
{
    RTTI_DECL;
    
public:
    static void create();
    
    static void destroy();
    
    static Chapter1Level1* getInstance();
    
private:
    static Chapter1Level1* s_pInstance;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level1(const char* json);
    Chapter1Level1(const Chapter1Level1&);
    Chapter1Level1& operator=(const Chapter1Level1&);
};

class Chapter1Level2 : public Level
{
    RTTI_DECL;
    
public:
    static void create();
    
    static void destroy();
    
    static Chapter1Level2* getInstance();
    
private:
    static Chapter1Level2* s_pInstance;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level2(const char* json);
    Chapter1Level2(const Chapter1Level2&);
    Chapter1Level2& operator=(const Chapter1Level2&);
};

class Chapter1Level3 : public Level
{
    RTTI_DECL;
    
public:
    static void create();
    
    static void destroy();
    
    static Chapter1Level3* getInstance();
    
private:
    static Chapter1Level3* s_pInstance;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level3(const char* json);
    Chapter1Level3(const Chapter1Level3&);
    Chapter1Level3& operator=(const Chapter1Level3&);
};

class Chapter1Level4 : public Level
{
    RTTI_DECL;
    
public:
    static void create();
    
    static void destroy();
    
    static Chapter1Level4* getInstance();
    
private:
    static Chapter1Level4* s_pInstance;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level4(const char* json);
    Chapter1Level4(const Chapter1Level4&);
    Chapter1Level4& operator=(const Chapter1Level4&);
};

class Chapter1Level5 : public Level
{
    RTTI_DECL;
    
public:
    static void create();
    
    static void destroy();
    
    static Chapter1Level5* getInstance();
    
private:
    static Chapter1Level5* s_pInstance;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level5(const char* json);
    Chapter1Level5(const Chapter1Level5&);
    Chapter1Level5& operator=(const Chapter1Level5&);
};

class Chapter1Level6 : public Level
{
    RTTI_DECL;
    
public:
    static void create();
    
    static void destroy();
    
    static Chapter1Level6* getInstance();
    
private:
    static Chapter1Level6* s_pInstance;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level6(const char* json);
    Chapter1Level6(const Chapter1Level6&);
    Chapter1Level6& operator=(const Chapter1Level6&);
};

class Chapter1Level7 : public Level
{
    RTTI_DECL;
    
public:
    static void create();
    
    static void destroy();
    
    static Chapter1Level7* getInstance();
    
private:
    static Chapter1Level7* s_pInstance;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level7(const char* json);
    Chapter1Level7(const Chapter1Level7&);
    Chapter1Level7& operator=(const Chapter1Level7&);
};

class Chapter1Level8 : public Level
{
    RTTI_DECL;
    
public:
    static void create();
    
    static void destroy();
    
    static Chapter1Level8* getInstance();
    
private:
    static Chapter1Level8* s_pInstance;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level8(const char* json);
    Chapter1Level8(const Chapter1Level8&);
    Chapter1Level8& operator=(const Chapter1Level8&);
};

class Chapter1Level9 : public Level
{
    RTTI_DECL;
    
public:
    static void create();
    
    static void destroy();
    
    static Chapter1Level9* getInstance();
    
private:
    static Chapter1Level9* s_pInstance;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level9(const char* json);
    Chapter1Level9(const Chapter1Level9&);
    Chapter1Level9& operator=(const Chapter1Level9&);
};

class Chapter1Level10 : public Level
{
    RTTI_DECL;
    
public:
    static void create();
    
    static void destroy();
    
    static Chapter1Level10* getInstance();
    
    virtual void enter(MainScreen* ms);
    
    virtual void exit(MainScreen* ms);
    
private:
    static Chapter1Level10* s_pInstance;
    
    MidBossOwl* m_midBossOwl;
    ForegroundObject* m_perchTree;
    float m_fJonY;
    float m_fGameStateTime;
    float m_fIdleWaitTime;
    float m_fMusicVolume;
    int m_iLastKnownOwlDamage;
    int m_iLastKnownJonNumBoosts;
    int m_iNumCarrotsCollectedAtCheckpoint;
    int m_iNumGoldenCarrotsCollectedAtCheckpoint;
    bool m_isIdleWaitingForOwl;
    bool m_hasTriggeredMidBossMusicLoopIntro;
    bool m_hasTriggeredMidBossMusicLoop;
    bool m_isChaseCamActivated;
    bool m_hasTriggeredBurrow;
    bool m_showHintBecauseJonHasBeenCaptured;
    bool m_hasShownHintPopup;
    
    virtual void update(MainScreen* ms);
    
    virtual void updateCamera(MainScreen* ms, float paddingX = 0, bool ignoreY = false, bool instant = false);
    
    virtual void additionalRenderingBeforeHud(MainScreen* ms);
    
    virtual bool isInSlowMotionMode();
    
    virtual void configBatPanel();
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level10(const char* json);
    virtual ~Chapter1Level10();
    Chapter1Level10(const Chapter1Level10&);
    Chapter1Level10& operator=(const Chapter1Level10&);
};

class Chapter1Level11 : public Level
{
    RTTI_DECL;
    
public:
    static void create();
    
    static void destroy();
    
    static Chapter1Level11* getInstance();
    
private:
    static Chapter1Level11* s_pInstance;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level11(const char* json);
    Chapter1Level11(const Chapter1Level11&);
    Chapter1Level11& operator=(const Chapter1Level11&);
};

class Chapter1Level12 : public Level
{
    RTTI_DECL;
    
public:
    static void create();
    
    static void destroy();
    
    static Chapter1Level12* getInstance();
    
private:
    static Chapter1Level12* s_pInstance;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level12(const char* json);
    Chapter1Level12(const Chapter1Level12&);
    Chapter1Level12& operator=(const Chapter1Level12&);
};

class Chapter1Level13 : public Level
{
    RTTI_DECL;
    
public:
    static void create();
    
    static void destroy();
    
    static Chapter1Level13* getInstance();
    
private:
    static Chapter1Level13* s_pInstance;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level13(const char* json);
    Chapter1Level13(const Chapter1Level13&);
    Chapter1Level13& operator=(const Chapter1Level13&);
};

class Chapter1Level14 : public Level
{
    RTTI_DECL;
    
public:
    static void create();
    
    static void destroy();
    
    static Chapter1Level14* getInstance();
    
private:
    static Chapter1Level14* s_pInstance;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level14(const char* json);
    Chapter1Level14(const Chapter1Level14&);
    Chapter1Level14& operator=(const Chapter1Level14&);
};

class Chapter1Level15 : public Level
{
    RTTI_DECL;
    
public:
    static void create();
    
    static void destroy();
    
    static Chapter1Level15* getInstance();
    
private:
    static Chapter1Level15* s_pInstance;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level15(const char* json);
    Chapter1Level15(const Chapter1Level15&);
    Chapter1Level15& operator=(const Chapter1Level15&);
};

class Chapter1Level16 : public Level
{
    RTTI_DECL;
    
public:
    static void create();
    
    static void destroy();
    
    static Chapter1Level16* getInstance();
    
private:
    static Chapter1Level16* s_pInstance;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level16(const char* json);
    Chapter1Level16(const Chapter1Level16&);
    Chapter1Level16& operator=(const Chapter1Level16&);
};

class Chapter1Level17 : public Level
{
    RTTI_DECL;
    
public:
    static void create();
    
    static void destroy();
    
    static Chapter1Level17* getInstance();
    
private:
    static Chapter1Level17* s_pInstance;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level17(const char* json);
    Chapter1Level17(const Chapter1Level17&);
    Chapter1Level17& operator=(const Chapter1Level17&);
};

class Chapter1Level18 : public Level
{
    RTTI_DECL;
    
public:
    static void create();
    
    static void destroy();
    
    static Chapter1Level18* getInstance();
    
private:
    static Chapter1Level18* s_pInstance;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level18(const char* json);
    Chapter1Level18(const Chapter1Level18&);
    Chapter1Level18& operator=(const Chapter1Level18&);
};

class Chapter1Level19 : public Level
{
    RTTI_DECL;
    
public:
    static void create();
    
    static void destroy();
    
    static Chapter1Level19* getInstance();
    
private:
    static Chapter1Level19* s_pInstance;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level19(const char* json);
    Chapter1Level19(const Chapter1Level19&);
    Chapter1Level19& operator=(const Chapter1Level19&);
};

class Chapter1Level20 : public Level
{
    RTTI_DECL;
    
public:
    static void create();
    
    static void destroy();
    
    static Chapter1Level20* getInstance();
    
private:
    static Chapter1Level20* s_pInstance;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level20(const char* json);
    Chapter1Level20(const Chapter1Level20&);
    Chapter1Level20& operator=(const Chapter1Level20&);
};

class Chapter1Level21 : public Level
{
    RTTI_DECL;
    
public:
    static void create();
    
    static void destroy();
    
    static Chapter1Level21 * getInstance();
    
    virtual void enter(MainScreen* ms);
    
    virtual void exit(MainScreen* ms);
    
private:
    static Chapter1Level21* s_pInstance;
    
    EndBossSnake* m_endBossSnake;
    Hole* m_hole;
	float m_fGameStateTime;
	float m_fCheckPointStateTime;
	float m_fCheckPointX;
	float m_fCheckPointY;
	float m_fMarker1X;
	float m_fMarker2X;
	float m_fMarker3X;
	float m_fMusicVolume;
    float m_fInitialDistanceToHole;
	int m_iNumCarrotsCollectedAtCheckpoint;
	int m_iNumGoldenCarrotsCollectedAtCheckpoint;
    bool m_isChaseCamActivated;
	bool m_hasTriggeredMusicLoopIntro;
	bool m_hasTriggeredSnakeAwaken;
	bool m_hasTriggeredMusicLoop;
	bool m_hasTriggeredCheckPoint;
	bool m_hasTriggeredSnakeDeathCheckPoint;

    virtual void onEnter(MainScreen* ms);
    
	virtual void beginOpeningSequence(MainScreen* ms);

	virtual void handleOpeningSequence(MainScreen* ms);
    
    virtual void update(MainScreen* ms);
    
    virtual void updateCamera(MainScreen* ms, float paddingX = 0, bool ignoreY = false, bool instant = false);
    
    virtual bool isInSlowMotionMode();

    virtual void configBatPanel();
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level21(const char* json);
    Chapter1Level21(const Chapter1Level21&);
    Chapter1Level21& operator=(const Chapter1Level21&);
};

class LevelUtil
{
    RTTI_DECL;
    
public:
    static Level* getInstanceForWorldAndLevel(int world, int level);
    
    static std::vector<UserDemoAction> getUserDemoActionsForWorldAndLevel(int world, int level);
};

#endif /* defined(__nosfuratu__MainScreenLevels__) */
