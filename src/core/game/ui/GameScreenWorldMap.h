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

class LevelThumbnail : public PhysicalEntity
{
public:
    static LevelThumbnail* create(LevelThumbnailType type);
    
    LevelThumbnail(float x, float y, float width, float height, float selectTime, int world, int level, LevelThumbnailType type) : PhysicalEntity(x, y, width, height),
    m_type(type),
    m_fSelectTime(selectTime),
    m_iWorld(world),
    m_iLevel(level),
    m_isPlayable(false),
    m_isSelecting(false),
    m_isSelected(false),
    m_isClearing(false),
    m_isCleared(false)
    {
        // Empty
    }
    
    virtual void update(float deltaTime)
    {
        if (m_isClearing)
        {
            m_fStateTime += deltaTime;
            
            if (m_fStateTime > 1.10f)
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
    
    int getWorld() { return m_iWorld; }
    
    int getLevel() { return m_iLevel; }
    
    void config(bool isPlayable, bool isClearing, bool isCleared)
    {
        m_isPlayable = isPlayable;
        m_isSelecting = false;
        m_isSelected = false;
        m_isClearing = isClearing;
        m_isCleared = isCleared;
        
        m_fStateTime = 0;
    }
    
    void select()
    {
        m_isSelecting = true;
        m_isSelected = false;
        
        m_fStateTime = 0;
    }
    
    void deselect()
    {
        m_isSelecting = false;
        m_isSelected = false;
        
        m_fStateTime = 0;
    }
    
    bool isPlayable() { return m_isPlayable; }
    
    bool isSelecting() { return m_isSelecting; }
    
    bool isSelected() { return m_isSelected; }
    
    bool isClearing() { return m_isClearing; }
    
    bool isCleared() { return m_isCleared; }
    
private:
    LevelThumbnailType m_type;
    float m_fSelectTime;
    int m_iWorld;
    int m_iLevel;
    bool m_isPlayable;
    bool m_isSelecting;
    bool m_isSelected;
    bool m_isClearing;
    bool m_isCleared;
};

class NormalLevelThumbnail : public LevelThumbnail
{
public:
    NormalLevelThumbnail(float x, float y, int world, int level) : LevelThumbnail(x, y, CAM_WIDTH * 0.11397058823529f, CAM_HEIGHT * 0.20261437908497f, 0.6f, world, level, LevelThumbnailType_Normal) {}
};

class BossLevelThumbnail : public LevelThumbnail
{
public:
    BossLevelThumbnail(float x, float y, int world, int level) : LevelThumbnail(x, y, CAM_WIDTH * 0.08639705882353f, CAM_HEIGHT * 0.22549019607843f, 0.4f, world, level, LevelThumbnailType_Boss) {}
};

class GoldenCarrotsMarker : public PhysicalEntity
{
public:
    GoldenCarrotsMarker() : PhysicalEntity(1337, 1337, CAM_WIDTH * 0.10845588235294f, CAM_HEIGHT * 0.12091503267974f), m_iNumGoldenCarrots(0)
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
    
    int getNumGoldenCarrots() { return m_iNumGoldenCarrots; }
    
private:
    int m_iNumGoldenCarrots;
};

class WorldMap : public State<GameScreen>
{
public:
    static WorldMap* getInstance();
    
    virtual void enter(GameScreen* gs);
    
    virtual void execute(GameScreen* gs);
    
    virtual void exit(GameScreen* gs);
    
    void loadUserSaveData(const char* json);
    
    WorldMapPanel* getWorldMapPanel();
    
    std::vector<LevelThumbnail*>& getLevelThumbnails();
    
    GameButton* getBackButton();
    
    GameButton* getLeaderBoardsButton();
    
    int getViewedCutsceneFlag();
    
private:
    std::unique_ptr<WorldMapPanel> m_panel;
    std::unique_ptr<GoldenCarrotsMarker> m_goldenCarrotsMarker;
    std::vector<std::unique_ptr<WorldLevelCompletions>> m_worldLevelStats;
    std::vector<LevelThumbnail*> m_levelThumbnails;
    std::unique_ptr<GameButton> m_backButton;
    std::unique_ptr<GameButton> m_leaderBoardsButton;
    int m_iNumCollectedGoldenCarrots;
    int m_iJonAbilityFlag;
    int m_iViewedCutsceneFlag;
    bool m_isReadyForTransition;
    
    void loadGlobalUserSaveData(rapidjson::Document& d);
    
    void loadUserSaveDataForWorld(rapidjson::Document& d, const char * key);
    
    void selectLevel(LevelThumbnail* levelThumbnail, int levelStatsFlag);
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    WorldMap();
    WorldMap(const WorldMap&);
    WorldMap& operator=(const WorldMap&);
};

#endif /* defined(__nosfuratu__GameScreenWorldMap__) */
