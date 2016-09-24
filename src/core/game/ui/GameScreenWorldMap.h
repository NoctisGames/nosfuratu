//
//  GameScreenWorldMap.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/10/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__GameScreenWorldMap__
#define __nosfuratu__GameScreenWorldMap__

#include "State.h"
#include "GameButton.h"
#include "GameScreenLevels.h"
#include "WorldMapPanel.h"

#include "rapidjson/document.h"

#include <memory>
#include <vector>

class GameScreen;

class WorldLevelCompletions
{
public:
    WorldLevelCompletions() { };
    
    std::vector<int> m_levelStats;
    std::vector<int> m_scores;
    std::vector<int> m_onlineScores;
};

typedef enum
{
    LevelThumbnailType_Normal,
    LevelThumbnailType_Boss
} LevelThumbnailType;

typedef enum
{
    AbilitySlotType_Drill,
    AbilitySlotType_Dash
} AbilitySlotType;

class LevelThumbnail : public PhysicalEntity
{
public:
    static LevelThumbnail* create(LevelThumbnailType type);
    
    LevelThumbnail(float x, float y, float width, float height, float selectTime, float clearTime, int world, int level, LevelThumbnailType type) : PhysicalEntity(x, y, width, height),
    m_color(1, 1, 1, 1),
    m_type(type),
    m_fSelectTime(selectTime),
    m_fClearTime(clearTime),
    m_iWorld(world),
    m_iLevel(level),
    m_isPlayable(false),
    m_isSelecting(false),
    m_isSelected(false),
    m_isClearing(false),
    m_isCleared(false),
    m_needsToPlayClearSound(false)
    {
        // Empty
    }
    
    virtual void update(float deltaTime)
    {
        if (m_needsToPlayClearSound)
        {
            Assets::getInstance()->addSoundIdToPlayQueue(m_type == LevelThumbnailType_Boss ? SOUND_BOSS_LEVEL_CLEAR : SOUND_LEVEL_CLEAR);
            
            m_needsToPlayClearSound = false;
        }
        
        if (m_isClearing)
        {
            m_fStateTime += deltaTime;
            
            if (m_fStateTime > m_fClearTime)
            {
                m_isClearing = false;
                
                m_fStateTime = 0;
            }
        }
        else if (m_isSelecting)
        {
            m_fStateTime += deltaTime;
            
            if (m_fStateTime > m_fSelectTime)
            {
                m_isSelected = true;
                m_isSelecting = false;
                
                m_fStateTime = 0;
            }
        }
        else if (m_isSelected)
        {
            m_fStateTime += deltaTime;
        }
    }
    
    Color& getColor() { return m_color; }
    
    int getWorld() { return m_iWorld; }
    
    int getLevel() { return m_iLevel; }
    
    void config(bool isPlayable, bool isClearing, bool isCleared)
    {
        m_isPlayable = isPlayable;
        m_isSelecting = false;
        m_isSelected = false;
        m_isClearing = isClearing;
        m_isCleared = isCleared;
        
        m_needsToPlayClearSound = m_isClearing;
        
        m_fStateTime = 0;
    }
    
    virtual void select()
    {
        m_isSelecting = true;
        m_isSelected = false;
        
        m_fStateTime = 0;
    }
    
    virtual void deselect()
    {
        m_isSelecting = false;
        m_isSelected = false;
        
        m_fStateTime = 0;
    }
    
    bool isPlayable() { return m_isPlayable; }
    
    bool isSelecting() { return m_isSelecting; }
    
    bool isSelected() { return m_isSelected; }
    
    void onConfirm()
    {
        Assets::getInstance()->addSoundIdToPlayQueue(SOUND_LEVEL_CONFIRMED);
        
        m_fStateTime = 0;
    }
    
    bool isClearing() { return m_isClearing; }
    
    bool isCleared() { return m_isCleared; }
    
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
public:
    NormalLevelThumbnail(float x, float y, int world, int level) : LevelThumbnail(x, y, CAM_WIDTH * 0.11397058823529f, CAM_HEIGHT * 0.20261437908497f, 0.6f, 1.10f, world, level, LevelThumbnailType_Normal) {}
};

class SpendGoldenCarrotsBubble : public PhysicalEntity
{
public:
    SpendGoldenCarrotsBubble() : PhysicalEntity(1337, 1337, CAM_WIDTH * 0.21507352941176f, CAM_HEIGHT * 0.30065359477124f),
    m_color(1, 1, 1, 1),
    m_iWorld(-1),
    m_iLevel(-1),
    m_userHasEnoughGoldenCats(false),
    m_isOpen(false)
    {
        // Empty
    }
    
    virtual void update(float deltaTime)
    {
        if (m_isOpen)
        {
            m_fStateTime += deltaTime;
        }
    }
    
    /**
     * Return 1 if user spent golden carrots to unlock a boss level; 0 otherwise
     * Return 2 if the user didn't even touch the bubble at all
     */
    int handleTouch(Vector2D& touchPoint)
    {
        if (OverlapTester::isPointInRectangle(touchPoint, getMainBounds()))
        {
            if (m_userHasEnoughGoldenCats)
            {
                return 1;
            }
            
            return 0;
        }
        
        return 2;
    }
    
    void setUserHasEnoughGoldenCats(bool userHasEnoughGoldenCats)
    {
        m_userHasEnoughGoldenCats = userHasEnoughGoldenCats;
    }
    
    void config(float x, float y, int world, int level)
    {
        m_position->set(x, y);
        
        m_iWorld = world;
        m_iLevel = level;
        
        updateBounds();
        
        m_fStateTime = 0;
        
        m_isOpen = true;
    }
    
    void close()
    {
        m_position->set(1337, 1337);
        
        updateBounds();
        
        m_fStateTime = 0;
        
        m_isOpen = false;
    }
    
    Color& getColor() { return m_color; }
    
    int getWorld() { return m_iWorld; }
    
    int getLevel() { return m_iLevel; }
 
    bool userHasEnoughGoldenCats() { return m_userHasEnoughGoldenCats; }
    
    bool isOpen() { return m_isOpen; }
    
private:
    Color m_color;
    int m_iWorld;
    int m_iLevel;
    bool m_userHasEnoughGoldenCats;
    bool m_isOpen;
};

class BossLevelThumbnail : public LevelThumbnail
{
public:
    BossLevelThumbnail(float x, float y, int world, int level, SpendGoldenCarrotsBubble& spendGoldenCarrotsBubble) : LevelThumbnail(x, y, CAM_WIDTH * 0.18198529411765f, CAM_HEIGHT * 0.33333333333333f, 0.3f, 0.7f, world, level, LevelThumbnailType_Boss), m_spendGoldenCarrotsBubble(spendGoldenCarrotsBubble), m_isJawMoving(false), m_isUnlocking(false), m_isUnlocked(false), m_needsToPlayUnlockSound(false) {}
    
    virtual void update(float deltaTime)
    {
        if (m_needsToPlayUnlockSound)
        {
            Assets::getInstance()->addSoundIdToPlayQueue(SOUND_BOSS_LEVEL_UNLOCK);
            
            m_needsToPlayUnlockSound = false;
        }
        
        if (m_isUnlocking)
        {
            m_fStateTime += deltaTime;
            
            if (m_fStateTime > 1.30f)
            {
                m_isUnlocking = false;
                
                m_fStateTime = 0;
                
                if (m_isSelecting)
                {
                    m_isSelected = true;
                    m_isSelecting = false;
                    
                    m_fStateTime = 0;
                }
            }
        }
        else if (m_isUnlocked)
        {
            LevelThumbnail::update(deltaTime);
        }
        else
        {
            m_fStateTime += deltaTime;
        }
    }
    
    virtual void select()
    {
        if (m_isUnlocked)
        {
            LevelThumbnail::select();
        }
        else
        {
            m_isJawMoving = true;
            
            m_fStateTime = 0;
            
            m_spendGoldenCarrotsBubble.config(m_position->getX() - m_fWidth / 2, m_position->getY() + m_fHeight / 2, getWorld(), getLevel());
        }
    }
    
    virtual void deselect()
    {
        if (m_isUnlocked)
        {
            LevelThumbnail::deselect();
        }
        else
        {
            m_isJawMoving = false;
            
            m_fStateTime = 0;
        }
        
        m_spendGoldenCarrotsBubble.close();
    }
    
    void configLockStatus(bool isUnlocked, bool isUnlocking)
    {
        m_isUnlocked = isUnlocked;
        m_isUnlocking = isUnlocking;
        
        m_needsToPlayUnlockSound = m_isUnlocking;
        
        m_fStateTime = 0;
    }
    
    bool isJawMoving() { return m_isJawMoving; }
    
    bool isUnlocking() { return m_isUnlocking; }
    
    bool isUnlocked() { return m_isUnlocked; }
    
private:
    SpendGoldenCarrotsBubble& m_spendGoldenCarrotsBubble;
    bool m_isJawMoving;
    bool m_isUnlocking;
    bool m_isUnlocked;
    bool m_needsToPlayUnlockSound;
};

class AbilitySlot : public PhysicalEntity
{
public:
    AbilitySlot(float x, float y, AbilitySlotType type) : PhysicalEntity(x, y, CAM_WIDTH * 0.10845588235294f, CAM_HEIGHT * 0.18627450980392f),
    m_color(1, 1, 1, 1),
    m_type(type),
    m_isUnlocked(false),
    m_isUnlocking(false),
    m_isRevealing(false),
    m_needsToPlayUnlockSound(false)
    {
        // Empty
    }
    
    virtual void update(float deltaTime)
    {
        if (m_needsToPlayUnlockSound)
        {
            Assets::getInstance()->addSoundIdToPlayQueue(SOUND_ABILITY_UNLOCK);
            
            m_needsToPlayUnlockSound = false;
        }
        
        if (m_isUnlocking)
        {
            m_fStateTime += deltaTime;
            
            if (m_fStateTime > 1.00f)
            {
                m_isUnlocking = false;
                m_isRevealing = true;
                
                m_fStateTime = 0;
            }
        }
        else if (m_isRevealing)
        {
            m_fStateTime += deltaTime;
            
            if (m_fStateTime > 1.10f)
            {
                m_isRevealing = false;
            }
        }
    }
    
    Color& getColor() { return m_color; }
    
    AbilitySlotType getType() { return m_type; }
    
    bool isUnlocked() { return m_isUnlocked; }
    
    bool isUnlocking() { return m_isUnlocking; }
    
    void config(bool isUnlocked, bool isUnlocking)
    {
        m_fStateTime = 0.0f;
        m_isUnlocked = isUnlocked;
        m_isUnlocking = isUnlocking;
        m_isRevealing = false;
        
        m_needsToPlayUnlockSound = m_isUnlocking;
        
        if (m_isUnlocked && !m_isUnlocking)
        {
            m_fStateTime = 1.10f;
        }
    }
    
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
public:
    GoldenCarrotsMarker() : PhysicalEntity(1337, 1337, CAM_WIDTH * 0.10845588235294f, CAM_HEIGHT * 0.12091503267974f), m_color(1, 1, 1, 1), m_iNumGoldenCarrots(0)
    {
        // Empty
    }
    
    virtual void update(float deltaTime)
    {
        m_fStateTime += deltaTime;
    }
    
    void config(float x, float y, int numGoldenCarrots)
    {
        m_position->set(x, y);
        
        m_iNumGoldenCarrots = numGoldenCarrots;
        
        m_fStateTime = 0;
    }
    
    Color& getColor() { return m_color; }
    
    int getNumGoldenCarrots() { return m_iNumGoldenCarrots; }
    
    void onConfirm() { update(1337); }
    
private:
    Color m_color;
    int m_iNumGoldenCarrots;
};

class ScoreMarker
{
public:
    ScoreMarker() :
    m_color(1, 1, 1, 0),
    m_fX(0),
    m_fY(0),
    m_iScore(0)
    {
        // Empty
    }
    
    void update(float deltaTime)
    {
        m_color.alpha += deltaTime;
        
        if (m_color.alpha > 1)
        {
            m_color.alpha = 1;
        }
    }
    
    void config(float x, float y, int score)
    {
        m_fX = x;
        m_fY = y;
        m_iScore = score;
        
        m_color.alpha = 0;
    }
    
    Color& getColor() { return m_color; }
    
    float getX() { return m_fX; }
    
    float getY() { return m_fY; }
    
    int getScore() { return m_iScore; }
    
    void onConfirm() { update(1337); }
    
private:
    Color m_color;
    float m_fX;
    float m_fY;
    int m_iScore;
};

class WorldMap : public State<GameScreen>
{
public:
    static WorldMap* getInstance();
    
    virtual void enter(GameScreen* gs);
    
    virtual void execute(GameScreen* gs);
    
    virtual void exit(GameScreen* gs);
    
    void loadUserSaveData(const char* json);
    
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
    
    GameButton* getNextWorldButton();
    
    float getGoldenCarrotCountFlickerTime() { return m_fGoldenCarrotCountFlickerTime; }
    
    int getNumCollectedGoldenCarrots();
    
    int getViewedCutsceneFlag();
    
    int getUnlockedLevelStatsFlag();
    
private:
    std::unique_ptr<WorldMapPanel> m_panel;
    std::unique_ptr<GoldenCarrotsMarker> m_goldenCarrotsMarker;
    std::unique_ptr<ScoreMarker> m_scoreMarker;
    std::unique_ptr<SpendGoldenCarrotsBubble> m_spendGoldenCarrotsBubble;
    std::vector<std::unique_ptr<WorldLevelCompletions>> m_worldLevelStats;
    std::vector<AbilitySlot*> m_abilitySlots;
    std::vector<LevelThumbnail*> m_levelThumbnails;
    std::unique_ptr<GameButton> m_backButton;
    std::unique_ptr<GameButton> m_toggleMusic;
    std::unique_ptr<GameButton> m_toggleSound;
    std::unique_ptr<GameButton> m_leaderBoardsButton;
    std::unique_ptr<GameButton> m_viewOpeningCutsceneButton;
    std::unique_ptr<GameButton> m_nextWorldButton;
    float m_fGoldenCarrotCountFlickerTime;
    int m_iNumCollectedGoldenCarrots;
    int m_iJonAbilityFlag;
    int m_iUnlockedLevelStatsFlag;
    int m_iViewedCutsceneFlag;
    bool m_isReadyForTransition;
    bool m_needsRefresh;
    bool m_isNextWorldButtonEnabled;
    
    LevelThumbnail* m_clickedLevel;
    bool m_userHasClickedOpeningCutscene;
    
    void loadGlobalUserSaveData(rapidjson::Document& d);
    
    void loadUserSaveDataForWorld(rapidjson::Document& d, const char * key);
    
    void configAbilitySlot(AbilitySlotType abilitySlotType, bool isUnlocked, bool isUnlocking);
    
    void selectLevel(LevelThumbnail* levelThumbnail, int levelStatsFlag, int score);
    
    void validateAbilityFlag();
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    WorldMap();
    WorldMap(const WorldMap&);
    WorldMap& operator=(const WorldMap&);
};

#endif /* defined(__nosfuratu__GameScreenWorldMap__) */
