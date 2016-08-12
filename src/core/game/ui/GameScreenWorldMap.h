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
    
    LevelThumbnail(float x, float y, float width, float height, int world, int level, LevelThumbnailType type) : PhysicalEntity(x, y, width, height),
    m_type(type),
    m_fAnimationDelay(0),
    m_iWorld(world),
    m_iLevel(level),
    m_isCompleted(false)
    {
        // Prevent animation
        m_fStateTime = 1;
    }
    
    virtual void update(float deltaTime)
    {
        m_fAnimationDelay -= deltaTime;
        
        if (m_fAnimationDelay < 0)
        {
            PhysicalEntity::update(deltaTime);
        }
    }
    
    int getWorld() { return m_iWorld; }
    
    int getLevel() { return m_iLevel; }
    
    bool isCompleted() { return m_isCompleted; }
    
    void setCompleted(bool isCompleted) { m_isCompleted = isCompleted; }
    
    void animate(float delay) { m_fAnimationDelay = delay; m_fStateTime = 0; }
    
private:
    LevelThumbnailType m_type;
    float m_fAnimationDelay;
    int m_iWorld;
    int m_iLevel;
    bool m_isCompleted;
};

class NormalLevelThumbnail : public LevelThumbnail
{
public:
    NormalLevelThumbnail(float x, float y, int world, int level) : LevelThumbnail(x, y, CAM_WIDTH * 0.11397058823529f, CAM_HEIGHT * 0.20261437908497f, world, level, LevelThumbnailType_Normal) {}
};

class BossLevelThumbnail : public LevelThumbnail
{
public:
    BossLevelThumbnail(float x, float y, int world, int level) : LevelThumbnail(x, y, CAM_WIDTH * 0.08639705882353f, CAM_HEIGHT * 0.22549019607843f, world, level, LevelThumbnailType_Boss) {}
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
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    WorldMap();
    WorldMap(const WorldMap&);
    WorldMap& operator=(const WorldMap&);
};

#endif /* defined(__nosfuratu__GameScreenWorldMap__) */
