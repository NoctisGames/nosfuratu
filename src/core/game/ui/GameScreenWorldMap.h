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
    m_iGoldenCarrotsFlag(0),
    m_isVisible(false),
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
    
    int getGoldenCarrotsFlag() { return m_iGoldenCarrotsFlag; }
    
    bool isVisible() { return m_isVisible; }
    
    bool isCompleted() { return m_isCompleted; }
    
    void setVisible(bool isVisible) { m_isVisible = isVisible; }
    
    void setLevelStats(int goldenCarrotsFlag) { m_iGoldenCarrotsFlag = goldenCarrotsFlag; }
    
    void setCompleted(bool isCompleted) { m_isCompleted = isCompleted; }
    
    void animate(float delay) { m_fAnimationDelay = delay; m_fStateTime = 0; }
    
private:
    LevelThumbnailType m_type;
    float m_fAnimationDelay;
    int m_iWorld;
    int m_iLevel;
    int m_iGoldenCarrotsFlag;
    bool m_isVisible;
    bool m_isCompleted;
};

class NormalLevelThumbnail : public LevelThumbnail
{
public:
    NormalLevelThumbnail(float x, float y, int world, int level) : LevelThumbnail(x, y, 1.0546875f, 1.0546875f, world, level, LevelThumbnailType_Normal) {}
};

class BossLevelThumbnail : public LevelThumbnail
{
public:
    BossLevelThumbnail(float x, float y, int world, int level) : LevelThumbnail(x, y, 0.650390625f, 1.212890625f, world, level, LevelThumbnailType_Boss) {}
};

typedef enum
{
    ABILITY_SLOT_LOCKED,
    ABILITY_SLOT_RABBIT_RIGHT,
    ABILITY_SLOT_RABBIT_UP,
    ABILITY_SLOT_RABBIT_LEFT,
    ABILITY_SLOT_RABBIT_DOWN,
    ABILITY_SLOT_VAMPIRE_RIGHT,
    ABILITY_SLOT_VAMPIRE_UP,
    ABILITY_SLOT_VAMPIRE_LEFT,
    ABILITY_SLOT_VAMPIRE_DOWN
} WorldMapMenuAbilitySlot_Type;

class WorldMapMenuAbilitySlot : public PhysicalEntity
{
public:
    WorldMapMenuAbilitySlot(float x, float y) : PhysicalEntity(x, y, CAM_WIDTH * 0.04296875f, CAM_HEIGHT * 0.07638888888889f),
    m_type(ABILITY_SLOT_LOCKED) {}
    
    virtual void update(float deltaTime)
    {
        PhysicalEntity::update(deltaTime);
    }
    
    void setWorldMapMenuAbilitySlotType(WorldMapMenuAbilitySlot_Type type) { m_type = type; }
    
    WorldMapMenuAbilitySlot_Type getWorldMapMenuAbilitySlotType() { return m_type; }
    
private:
    WorldMapMenuAbilitySlot_Type m_type;
};

class WorldMapGoldenCarrot : public PhysicalEntity
{
public:
    WorldMapGoldenCarrot(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height) {}
};

class WorldMapMenu : public PhysicalEntity
{
public:
    WorldMapMenu() : PhysicalEntity(CAM_WIDTH / 2, CAM_HEIGHT * 0.12847222222222f, CAM_WIDTH * 0.921875f, CAM_HEIGHT * 0.22916666666667f),
    m_iWorld(-1),
    m_iLevel(-1),
    m_iGoldenCarrotsFlag(0)
    {
        float x = m_position->getX() - m_fWidth / 2;
        float y = m_position->getY() - m_fHeight / 2;
        
        m_abilitySlots.push_back(std::unique_ptr<WorldMapMenuAbilitySlot>(new WorldMapMenuAbilitySlot(x + m_fWidth * 0.13983050847458f, y + m_fHeight * 0.48484848484848f)));
        m_abilitySlots.push_back(std::unique_ptr<WorldMapMenuAbilitySlot>(new WorldMapMenuAbilitySlot(x + m_fWidth * 0.08898305084746f, y + m_fHeight * 0.72727272727273f)));
        m_abilitySlots.push_back(std::unique_ptr<WorldMapMenuAbilitySlot>(new WorldMapMenuAbilitySlot(x + m_fWidth * 0.03813559322034f, y + m_fHeight * 0.48484848484848f)));
        m_abilitySlots.push_back(std::unique_ptr<WorldMapMenuAbilitySlot>(new WorldMapMenuAbilitySlot(x + m_fWidth * 0.08898305084746f, y + m_fHeight * 0.27272727272727f)));
        
        m_abilitySlots.push_back(std::unique_ptr<WorldMapMenuAbilitySlot>(new WorldMapMenuAbilitySlot(x + m_fWidth * 0.96186440677966f, y + m_fHeight * 0.48484848484848f)));
        m_abilitySlots.push_back(std::unique_ptr<WorldMapMenuAbilitySlot>(new WorldMapMenuAbilitySlot(x + m_fWidth * 0.91101694915254f, y + m_fHeight * 0.72727272727273f)));
        m_abilitySlots.push_back(std::unique_ptr<WorldMapMenuAbilitySlot>(new WorldMapMenuAbilitySlot(x + m_fWidth * 0.86016949152542f, y + m_fHeight * 0.48484848484848f)));
        m_abilitySlots.push_back(std::unique_ptr<WorldMapMenuAbilitySlot>(new WorldMapMenuAbilitySlot(x + m_fWidth * 0.91101694915254f, y + m_fHeight * 0.27272727272727f)));
    }
    
    virtual void update(float deltaTime)
    {
        PhysicalEntity::update(deltaTime);
    }
    
    void setLevelStats(int world, int level, int goldenCarrotsFlag)
    {
        m_iWorld = world;
        m_iLevel = level;
        m_iGoldenCarrotsFlag = goldenCarrotsFlag;
        
        for (std::vector<std::unique_ptr<WorldMapMenuAbilitySlot>>::iterator i = m_abilitySlots.begin(); i != m_abilitySlots.end(); i++)
        {
            (*i)->setWorldMapMenuAbilitySlotType(ABILITY_SLOT_LOCKED);
        }
        
        if (m_iWorld >= 1 && m_iLevel >= 10)
        {
            m_abilitySlots.at(3)->setWorldMapMenuAbilitySlotType(ABILITY_SLOT_RABBIT_DOWN);
        }
        
        if (m_iWorld >= 2)
        {
            m_abilitySlots.at(4)->setWorldMapMenuAbilitySlotType(ABILITY_SLOT_VAMPIRE_RIGHT);
        }
        
        // TODO, decide when other abilities appear as unlocked
    }
    
    int getWorld() { return m_iWorld; }
    
    int getLevel() { return m_iLevel; }
    
    int getGoldenCarrotsFlag() { return m_iGoldenCarrotsFlag; }
    
    std::vector<std::unique_ptr<WorldMapMenuAbilitySlot>>& getAbilitySlots() { return m_abilitySlots; }
    
private:
    std::vector<std::unique_ptr<WorldMapMenuAbilitySlot>> m_abilitySlots;
    int m_iWorld;
    int m_iLevel;
    int m_iGoldenCarrotsFlag;
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
    
    WorldMapMenu* getWorldMapMenu();
    
    std::vector<std::unique_ptr<LevelThumbnail>>& getLevelThumbnails();
    
    GameButton* getBackButton();
    
    GameButton* getLeaderBoardsButton();
    
    float getCamPosY();
    
private:
    std::unique_ptr<WorldMapPanel> m_panel;
    std::unique_ptr<WorldMapMenu> m_menu;
    std::vector<std::unique_ptr<WorldLevelCompletions>> m_worldLevelStats;
    std::vector<std::unique_ptr<LevelThumbnail>> m_levelThumbnails;
    std::unique_ptr<GameButton> m_backButton;
    std::unique_ptr<GameButton> m_leaderBoardsButton;
    float m_fCamPosY;
    int m_iNumCollectedGoldenCarrots;
    bool m_isReadyForTransition;
    
    void loadUserSaveData(rapidjson::Document& d, const char * key);
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    WorldMap();
    WorldMap(const WorldMap&);
    WorldMap& operator=(const WorldMap&);
};

#endif /* defined(__nosfuratu__GameScreenWorldMap__) */
