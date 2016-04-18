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
#include "BackButton.h"
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
    
    std::vector<bool> m_levelCompletions;
};

class LevelThumbnail : public PhysicalEntity
{
public:
    LevelThumbnail(float x, float y, int world, int level) : PhysicalEntity(x, y, 0.66796875f, 0.544921875f),
    m_iWorld(world),
    m_iLevel(level),
    m_isVisible(false),
    m_isCompleted(false) { }
    
    int getWorld() { return m_iWorld; };
    
    int getLevel() { return m_iLevel; };
    
    bool isVisible() { return m_isVisible; };
    
    bool isCompleted() { return m_isCompleted; };
    
    void setVisible(bool isVisible) { m_isVisible = isVisible; };
    
    void setCompleted(bool isCompleted) { m_isCompleted = isCompleted; };
    
private:
    int m_iWorld;
    int m_iLevel;
    bool m_isVisible;
    bool m_isCompleted;
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
    
    std::vector<std::unique_ptr<LevelThumbnail>>& getLevelThumbnails();
    
    BackButton* getBackButton();
    
private:
    std::unique_ptr<WorldMapPanel> m_panel;
    std::vector<std::unique_ptr<WorldLevelCompletions>> m_worldLevelCompletions;
    std::vector<std::unique_ptr<LevelThumbnail>> m_levelThumbnails;
    std::unique_ptr<BackButton> m_backButton;
    bool m_isReadyForTransition;
    
    void loadUserSaveData(rapidjson::Document& d, const char * key);
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    WorldMap();
    WorldMap(const WorldMap&);
    WorldMap& operator=(const WorldMap&);
};

#endif /* defined(__nosfuratu__GameScreenWorldMap__) */
