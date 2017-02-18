//
//  MainScreenWorldMap.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/10/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__MainScreenWorldMap__
#define __nosfuratu__MainScreenWorldMap__

#include "MainScreen.h"

#include "GameButton.h"
#include "MainScreenLevels.h"
#include "WorldMapPanel.h"
#include "RTTI.h"
#include "SoundManager.h"

#include "rapidjson/document.h"

#include <memory>
#include <vector>

class WorldLevelCompletions
{
    RTTI_DECL;
    
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
    RTTI_DECL;
    
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
            SOUND_MANAGER->addSoundIdToPlayQueue(m_type == LevelThumbnailType_Boss ? SOUND_BOSS_LEVEL_CLEAR : SOUND_LEVEL_CLEAR);
            
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
    
    virtual void config(bool isPlayable, bool isClearing, bool isCleared)
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
        SOUND_MANAGER->addSoundIdToPlayQueue(SOUND_LEVEL_CONFIRMED);
        
        m_fStateTime = 0;
    }
    
    bool isClearing() { return m_isClearing; }
    
    bool isCleared() { return m_isCleared; }
    
    virtual bool isBoss() = 0;
    
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
    NormalLevelThumbnail(float x, float y, int world, int level) : LevelThumbnail(x, y, CAM_WIDTH * 0.11397058823529f, CAM_HEIGHT * 0.20261437908497f, 0.6f, 1.10f, world, level, LevelThumbnailType_Normal) {}
    
    virtual bool isBoss() { return false; }
};

class SpendGoldenCarrotsBubble : public PhysicalEntity
{
    RTTI_DECL;
    
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
        if (OverlapTester::isPointInNGRect(touchPoint, getMainBounds()))
        {
            if (m_userHasEnoughGoldenCats && touchPoint.getY() > (getMainBounds().getBottom() + getMainBounds().getHeight() / 2))
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
        m_position.set(x, y);
        
        m_iWorld = world;
        m_iLevel = level;
        
        updateBounds();
        
        m_fStateTime = 0;
        
        m_isOpen = true;
    }
    
    void close()
    {
        m_position.set(1337, 1337);
        
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
    RTTI_DECL;
    
public:
    BossLevelThumbnail(float x, float y, int world, int level, SpendGoldenCarrotsBubble& spendGoldenCarrotsBubble, float width = CAM_WIDTH * 0.18198529411765f, float height = CAM_HEIGHT * 0.33333333333333f) : LevelThumbnail(x, y, width, height, 0.3f, 0.7f, world, level, LevelThumbnailType_Boss), m_spendGoldenCarrotsBubble(spendGoldenCarrotsBubble), m_isJawMoving(false), m_isUnlocking(false), m_isUnlocked(false), m_needsToPlayUnlockSound(false) {}
    
    virtual bool isBoss() { return true; }
    
    virtual void update(float deltaTime)
    {
        if (m_needsToPlayUnlockSound)
        {
            SOUND_MANAGER->addSoundIdToPlayQueue(SOUND_BOSS_LEVEL_UNLOCK);
            
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

	virtual void config(bool isPlayable, bool isClearing, bool isCleared)
	{
		LevelThumbnail::config(isPlayable, isClearing, isCleared);

		m_color.red = m_isPlayable ? 1 : 0;
		m_color.green = m_isPlayable ? 1 : 0;
		m_color.blue = m_isPlayable ? 1 : 0;
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

			float width = CAM_WIDTH * 0.18198529411765f;
			float height = CAM_HEIGHT * 0.33333333333333f;
            
            m_spendGoldenCarrotsBubble.config(m_position.getX() - width * 0.28f, m_position.getY() + height * 0.44f, getWorld(), getLevel());
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

class MidBossLevelThumbnail : public BossLevelThumbnail
{
    RTTI_DECL;
    
public:
	MidBossLevelThumbnail(float x, float y, int world, int level, SpendGoldenCarrotsBubble& spendGoldenCarrotsBubble, float width = CAM_WIDTH * 0.18198529411765f * 4 / 5, float height = CAM_HEIGHT * 0.33333333333333f * 4 / 5) : BossLevelThumbnail(x, y, world, level, spendGoldenCarrotsBubble, width, height) {}
};

class AbilitySlot : public PhysicalEntity
{
    RTTI_DECL;
    
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
            SOUND_MANAGER->addSoundIdToPlayQueue(SOUND_ABILITY_UNLOCK);
            
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
    RTTI_DECL;
    
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
        m_position.set(x, y);
        
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
    RTTI_DECL;
    
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

class WorldMap : public MainScreenState
{
    RTTI_DECL;
    
public:
    static WorldMap* getInstance();
    
    virtual void enter(MainScreen* gs);
    
    virtual void execute(MainScreen* gs);
    
    virtual void exit(MainScreen* gs);
    
    void loadUserSaveData(const char* json);
    
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
    
    float getGoldenCarrotCountFlickerTime() { return m_fGoldenCarrotCountFlickerTime; }
    
    int getNumCollectedGoldenCarrots();
    
    int getViewedCutsceneFlag();
    
    int getUnlockedLevelStatsFlag();
    
    LevelThumbnail* getSelectedLevelThumbnail() { return m_clickedLevel; }
    
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
    float m_fGoldenCarrotCountFlickerTime;
    int m_iNumCollectedGoldenCarrots;
    int m_iJonAbilityFlag;
    int m_iUnlockedLevelStatsFlag;
    int m_iViewedCutsceneFlag;
    int m_iNumTimesVisitedSinceLastAdBreak;
    bool m_isReadyForTransition;
    bool m_needsRefresh;
    bool m_isNextWorldButtonEnabled;
    
    LevelThumbnail* m_clickedLevel;
    bool m_userHasClickedOpeningCutscene;
    
    void loadGlobalUserSaveData(rapidjson::Document& d);
    
    void loadUserSaveDataForWorld(rapidjson::Document& d, const char * key);
    
    void configAbilitySlot(AbilitySlotType abilitySlotType, bool isUnlocked, bool isUnlocking);
    
    void selectLevel(LevelThumbnail* levelThumbnail);
    
    void startLevel();
    
    void validateAbilityFlag();
    
    void navRight();
    
    void navUp();
    
    void navLeft();
    
    void navDown();
    
    LevelThumbnail * getLevelThumbnail(int world, int level);
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    WorldMap();
    WorldMap(const WorldMap&);
    WorldMap& operator=(const WorldMap&);
};

#endif /* defined(__nosfuratu__MainScreenWorldMap__) */
