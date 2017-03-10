//
//  MainScreenWorldMap.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/10/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__MainScreenWorldMap__
#define __nosfuratu__MainScreenWorldMap__

#include "MainScreenState.h"
#include "PhysicalEntity.h"

#include "LevelThumbnailType.h"
#include "AbilitySlotType.h"
#include "GameConstants.h"
#include "Color.h"

#include "RTTI.h"

#include <vector>

class MainScreen;
class GameButton;
class WorldMapPanel;

class AbilitySlot;
class GoldenCarrotsMarker;
class ScoreMarker;
class SpendGoldenCarrotsBubble;
class LevelThumbnail;
class WorldLevelCompletions;

class WorldMap : public MainScreenState
{
    RTTI_DECL;
    
public:
    static WorldMap* getInstance();
    
    virtual void enter(MainScreen* ms);
    
    virtual void execute(MainScreen* ms);
    
    virtual void exit(MainScreen* ms);
    
    virtual void initRenderer(MainScreen* ms);
    
    void updateButtons(float deltaTime);
    
    void setFade(float fade); // 0 to 1, where 1 is complete fade
    
    WorldMapPanel* getWorldMapPanel();
    
    std::vector<AbilitySlot*>& getAbilitySlots();
    
    GoldenCarrotsMarker* getGoldenCarrotsMarker();
    
    ScoreMarker* getScoreMarker();
    
    SpendGoldenCarrotsBubble* getSpendGoldenCarrotsBubble();
    
    std::vector<LevelThumbnail*>& getLevelThumbnails();
    
    GameButton* getBackButton();
    
    GameButton* getToggleMusicButton();
    
    GameButton* getToggleSoundButton();
    
    GameButton* getLeaderBoardsButton();
    
    GameButton* getViewOpeningCutsceneButton();
    
    float getGoldenCarrotCountFlickerTime();
    
    int getNumCollectedGoldenCarrots();
    
    int getViewedCutsceneFlag();
    
    int getUnlockedLevelStatsFlag();
    
    LevelThumbnail* getSelectedLevelThumbnail();
    
    void loadSaveData();
    
private:
    WorldMapPanel* m_panel;
    GoldenCarrotsMarker* m_goldenCarrotsMarker;
    ScoreMarker* m_scoreMarker;
    SpendGoldenCarrotsBubble* m_spendGoldenCarrotsBubble;
    GameButton* m_backButton;
    GameButton* m_toggleMusic;
    GameButton* m_toggleSound;
    GameButton* m_leaderBoardsButton;
    GameButton* m_viewOpeningCutsceneButton;
    std::vector<AbilitySlot*> m_abilitySlots;
    std::vector<LevelThumbnail*> m_levelThumbnails;
    std::vector<WorldLevelCompletions*> m_worldLevelStats;
    float m_fGoldenCarrotCountFlickerTime;
    int m_iNumCollectedGoldenCarrots;
    int m_iJonAbilityFlag;
    int m_iUnlockedLevelStatsFlag;
    int m_iViewedCutsceneFlag;
    int m_iNumTimesVisitedSinceLastAdBreak;
    int m_iStickNavDirection;
    bool m_isReadyForTransition;
    bool m_needsRefresh;
    bool m_isNextWorldButtonEnabled;
    
    LevelThumbnail* m_clickedLevel;
    bool m_userHasClickedOpeningCutscene;
    
    void loadGlobalSaveData();
    
    void loadWorld1SaveData();
    
    void configAbilitySlot(AbilitySlotType abilitySlotType, bool isUnlocked, bool isUnlocking);
    
    void selectLevel(LevelThumbnail* levelThumbnail);
    
    void startLevel();
    
    void validateAbilityFlag();
    
    void navRight();
    
    void navUp();
    
    void navLeft();
    
    void navDown();
    
    void navSelect(MainScreen* ms);
    
    void onButtonSelected();
    
    LevelThumbnail * getLevelThumbnail(int world, int level);
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    WorldMap();
    ~WorldMap();
    WorldMap(const WorldMap&);
    WorldMap& operator=(const WorldMap&);
};

class WorldLevelCompletions
{
    RTTI_DECL;
    
public:
    WorldLevelCompletions();
    
    std::vector<int> m_levelStats;
    std::vector<int> m_scores;
    std::vector<int> m_onlineScores;
};

class LevelThumbnail : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    LevelThumbnail(float x, float y, float width, float height, float selectTime, float clearTime, int world, int level, LevelThumbnailType type);
    
    virtual bool isBoss() = 0;
    
    virtual void update(float deltaTime);
    
    Color& getColor();
    
    int getWorld();
    
    int getLevel();
    
    virtual void config(bool isPlayable, bool isClearing, bool isCleared);
    
    virtual void select();
    
    virtual void deselect();
    
    bool isPlayable();
    
    bool isSelecting();
    
    bool isSelected();
    
    void onConfirm();
    
    bool isClearing();
    
    bool isCleared();
    
protected:
    Color m_color;
    LevelThumbnailType m_type;
    float m_fSelectTime;
    float m_fClearTime;
    int m_iWorld;
    int m_iLevel;
    bool m_isPlayable;
    bool m_isSelecting;
    bool m_isSelected;
    bool m_isClearing;
    bool m_isCleared;
    bool m_needsToPlayClearSound;
};

class NormalLevelThumbnail : public LevelThumbnail
{
    RTTI_DECL;
    
public:
    NormalLevelThumbnail(float x, float y, int world, int level);
    
    virtual bool isBoss();
};

class SpendGoldenCarrotsBubble : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    SpendGoldenCarrotsBubble();
    
    virtual void update(float deltaTime);
    
    /**
     * Return 1 if user spent golden carrots to unlock a boss level; 0 otherwise
     * Return 2 if the user didn't even touch the bubble at all
     */
    int handleTouch(Vector2D& touchPoint);
    
    void setUserHasEnoughGoldenCats(bool userHasEnoughGoldenCats);
    
    void config(float x, float y, int world, int level);
    
    void close();
    
    Color& getColor();
    
    int getWorld();
    
    int getLevel();
    
    bool userHasEnoughGoldenCats();
    
    bool isOpen();
    
private:
    Color m_color;
    int m_iWorld;
    int m_iLevel;
    bool m_userHasEnoughGoldenCats;
    bool m_isOpen;
};

class BossLevelThumbnail : public LevelThumbnail
{
    RTTI_DECL;
    
public:
    BossLevelThumbnail(float x, float y, int world, int level, SpendGoldenCarrotsBubble& spendGoldenCarrotsBubble, float width = CAM_WIDTH * 0.18198529411765f, float height = CAM_HEIGHT * 0.33333333333333f);
    
    virtual bool isBoss();
    
    virtual void update(float deltaTime);
    
    virtual void config(bool isPlayable, bool isClearing, bool isCleared);
    
    virtual void select();
    
    virtual void deselect();
    
    void configLockStatus(bool isUnlocked, bool isUnlocking);
    
    bool isJawMoving();
    
    bool isUnlocking();
    
    bool isUnlocked();
    
private:
    SpendGoldenCarrotsBubble& m_spendGoldenCarrotsBubble;
    bool m_isJawMoving;
    bool m_isUnlocking;
    bool m_isUnlocked;
    bool m_needsToPlayUnlockSound;
};

class MidBossLevelThumbnail : public BossLevelThumbnail
{
    RTTI_DECL;
    
public:
    MidBossLevelThumbnail(float x, float y, int world, int level, SpendGoldenCarrotsBubble& spendGoldenCarrotsBubble, float width = CAM_WIDTH * 0.18198529411765f * 4 / 5, float height = CAM_HEIGHT * 0.33333333333333f * 4 / 5);
};

class AbilitySlot : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    AbilitySlot(float x, float y, AbilitySlotType type);
    
    virtual void update(float deltaTime);
    
    Color& getColor();
    
    AbilitySlotType getType();
    
    bool isUnlocked();
    
    bool isUnlocking();
    
    void config(bool isUnlocked, bool isUnlocking);
    
private:
    Color m_color;
    AbilitySlotType m_type;
    bool m_isUnlocked;
    bool m_isUnlocking;
    bool m_isRevealing;
    bool m_needsToPlayUnlockSound;
};

class GoldenCarrotsMarker : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    GoldenCarrotsMarker();
    
    virtual void update(float deltaTime);
    
    void config(float x, float y, int numGoldenCarrots);
    
    Color& getColor();
    
    int getNumGoldenCarrots();
    
    void onConfirm();
    
private:
    Color m_color;
    int m_iNumGoldenCarrots;
};

class ScoreMarker
{
    RTTI_DECL;
    
public:
    ScoreMarker();
    
    void update(float deltaTime);
    
    void config(float x, float y, int score);
    
    Color& getColor();
    
    float getX();
    
    float getY();
    
    int getScore();
    
    void onConfirm();
    
private:
    Color m_color;
    float m_fX;
    float m_fY;
    int m_iScore;
};

#endif /* defined(__nosfuratu__MainScreenWorldMap__) */
