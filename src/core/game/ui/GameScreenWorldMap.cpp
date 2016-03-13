//
//  GameScreenWorldMap.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/10/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "GameScreenWorldMap.h"
#include "State.h"
#include "GameScreen.h"
#include "EntityUtils.h"
#include "Vector2D.h"
#include "Game.h"
#include "GameScreenLevelEditor.h"
#include "GameScreenTransitions.h"
#include "GameScreenTitle.h"

#define world_1_level_1_name "A Picnic Cut Short"
#define world_1_level_2_name "Insert Level Name Here"
#define world_1_level_3_name "Insert Level Name Here"
#define world_1_level_4_name "Insert Level Name Here"
#define world_1_level_5_name "Insert Level Name Here"
#define world_1_level_6_name "Insert Level Name Here"
#define world_1_level_7_name "Insert Level Name Here"
#define world_1_level_8_name "Insert Level Name Here"
#define world_1_level_9_name "Insert Level Name Here"
#define world_1_level_10_name "Insert Level Name Here"
#define world_1_level_11_name "Insert Level Name Here"
#define world_1_level_12_name "Insert Level Name Here"
#define world_1_level_13_name "Insert Level Name Here"
#define world_1_level_14_name "Insert Level Name Here"
#define world_1_level_15_name "Insert Level Name Here"
#define world_1_level_16_name "Insert Level Name Here"
#define world_1_level_17_name "Insert Level Name Here"
#define world_1_level_18_name "Insert Level Name Here"
#define world_1_level_19_name "Insert Level Name Here"
#define world_1_level_20_name "Insert Level Name Here"
#define world_1_level_21_name "Insert Level Name Here"

#define world_2_level_1_name "Insert Level Name Here"
#define world_2_level_2_name "Insert Level Name Here"
#define world_2_level_3_name "Insert Level Name Here"
#define world_2_level_4_name "Insert Level Name Here"
#define world_2_level_5_name "Insert Level Name Here"
#define world_2_level_6_name "Insert Level Name Here"
#define world_2_level_7_name "Insert Level Name Here"
#define world_2_level_8_name "Insert Level Name Here"
#define world_2_level_9_name "Insert Level Name Here"
#define world_2_level_10_name "Insert Level Name Here"
#define world_2_level_11_name "Insert Level Name Here"
#define world_2_level_12_name "Insert Level Name Here"
#define world_2_level_13_name "Insert Level Name Here"
#define world_2_level_14_name "Insert Level Name Here"
#define world_2_level_15_name "Insert Level Name Here"
#define world_2_level_16_name "Insert Level Name Here"
#define world_2_level_17_name "Insert Level Name Here"
#define world_2_level_18_name "Insert Level Name Here"
#define world_2_level_19_name "Insert Level Name Here"
#define world_2_level_20_name "Insert Level Name Here"
#define world_2_level_21_name "Insert Level Name Here"

#define world_3_level_1_name "Insert Level Name Here"
#define world_3_level_2_name "Insert Level Name Here"
#define world_3_level_3_name "Insert Level Name Here"
#define world_3_level_4_name "Insert Level Name Here"
#define world_3_level_5_name "Insert Level Name Here"
#define world_3_level_6_name "Insert Level Name Here"
#define world_3_level_7_name "Insert Level Name Here"
#define world_3_level_8_name "Insert Level Name Here"
#define world_3_level_9_name "Insert Level Name Here"
#define world_3_level_10_name "Insert Level Name Here"
#define world_3_level_11_name "Insert Level Name Here"
#define world_3_level_12_name "Insert Level Name Here"
#define world_3_level_13_name "Insert Level Name Here"
#define world_3_level_14_name "Insert Level Name Here"
#define world_3_level_15_name "Insert Level Name Here"
#define world_3_level_16_name "Insert Level Name Here"
#define world_3_level_17_name "Insert Level Name Here"
#define world_3_level_18_name "Insert Level Name Here"
#define world_3_level_19_name "Insert Level Name Here"
#define world_3_level_20_name "Insert Level Name Here"
#define world_3_level_21_name "Insert Level Name Here"

#define world_4_level_1_name "Insert Level Name Here"
#define world_4_level_2_name "Insert Level Name Here"
#define world_4_level_3_name "Insert Level Name Here"
#define world_4_level_4_name "Insert Level Name Here"
#define world_4_level_5_name "Insert Level Name Here"
#define world_4_level_6_name "Insert Level Name Here"
#define world_4_level_7_name "Insert Level Name Here"
#define world_4_level_8_name "Insert Level Name Here"
#define world_4_level_9_name "Insert Level Name Here"
#define world_4_level_10_name "Insert Level Name Here"
#define world_4_level_11_name "Insert Level Name Here"
#define world_4_level_12_name "Insert Level Name Here"
#define world_4_level_13_name "Insert Level Name Here"
#define world_4_level_14_name "Insert Level Name Here"
#define world_4_level_15_name "Insert Level Name Here"
#define world_4_level_16_name "Insert Level Name Here"
#define world_4_level_17_name "Insert Level Name Here"
#define world_4_level_18_name "Insert Level Name Here"
#define world_4_level_19_name "Insert Level Name Here"
#define world_4_level_20_name "Insert Level Name Here"
#define world_4_level_21_name "Insert Level Name Here"

#define world_5_level_1_name "Insert Level Name Here"
#define world_5_level_2_name "Insert Level Name Here"
#define world_5_level_3_name "Insert Level Name Here"
#define world_5_level_4_name "Insert Level Name Here"
#define world_5_level_5_name "Insert Level Name Here"
#define world_5_level_6_name "Insert Level Name Here"
#define world_5_level_7_name "Insert Level Name Here"
#define world_5_level_8_name "Insert Level Name Here"
#define world_5_level_9_name "Insert Level Name Here"
#define world_5_level_10_name "Insert Level Name Here"
#define world_5_level_11_name "Insert Level Name Here"
#define world_5_level_12_name "Insert Level Name Here"
#define world_5_level_13_name "Insert Level Name Here"
#define world_5_level_14_name "Insert Level Name Here"
#define world_5_level_15_name "Insert Level Name Here"
#define world_5_level_16_name "Insert Level Name Here"
#define world_5_level_17_name "Insert Level Name Here"
#define world_5_level_18_name "Insert Level Name Here"
#define world_5_level_19_name "Insert Level Name Here"
#define world_5_level_20_name "Insert Level Name Here"
#define world_5_level_21_name "Insert Level Name Here"

/// World Map ///

WorldMap * WorldMap::getInstance()
{
    static WorldMap *instance = new WorldMap();
    
    return instance;
}

void WorldMap::enter(GameScreen* gs)
{
	gs->m_stateMachine->setPreviousState(Title::getInstance());
    gs->m_renderer->init(RENDERER_TYPE_WORLD_MAP);
    
    gs->m_renderer->unload(RENDERER_TYPE_WORLD_1_PRE_LEVEL_10);
    gs->m_renderer->unload(RENDERER_TYPE_WORLD_1_MID_BOSS);
    gs->m_renderer->unload(RENDERER_TYPE_WORLD_1_POST_LEVEL_10);
    gs->m_renderer->unload(RENDERER_TYPE_WORLD_1_END_BOSS);
    
    gs->m_renderer->unload(RENDERER_TYPE_WORLD_2);
    gs->m_renderer->unload(RENDERER_TYPE_WORLD_2_MID_BOSS);
    gs->m_renderer->unload(RENDERER_TYPE_WORLD_2_END_BOSS);
    
    gs->m_renderer->unload(RENDERER_TYPE_WORLD_3);
    gs->m_renderer->unload(RENDERER_TYPE_WORLD_3_MID_BOSS);
    gs->m_renderer->unload(RENDERER_TYPE_WORLD_3_END_BOSS);
    
    gs->m_renderer->unload(RENDERER_TYPE_WORLD_4);
    gs->m_renderer->unload(RENDERER_TYPE_WORLD_4_MID_BOSS);
    gs->m_renderer->unload(RENDERER_TYPE_WORLD_4_END_BOSS);
    
    gs->m_renderer->unload(RENDERER_TYPE_WORLD_5);
    gs->m_renderer->unload(RENDERER_TYPE_WORLD_5_MID_BOSS);
    gs->m_renderer->unload(RENDERER_TYPE_WORLD_5_END_BOSS);
    
    gs->m_iRequestedAction = REQUESTED_ACTION_GET_LEVEL_COMPLETIONS;
}

void WorldMap::execute(GameScreen* gs)
{
    if (gs->m_isRequestingRender)
    {
        gs->m_renderer->beginFrame(gs->m_fDeltaTime);
        
        gs->m_renderer->renderWorldMapScreenBackground(m_panel.get());
        
        if (gs->m_renderer->isLoadingAdditionalTextures())
        {
            gs->m_renderer->renderLoading();
        }
        else
        {
            gs->m_renderer->renderWorldMapScreenUi(m_levelThumbnails, m_backButton.get());
        }
        
        gs->m_renderer->renderToScreen();
        
        gs->m_renderer->endFrame();
    }
    else
    {
        if (gs->m_renderer->isLoadingAdditionalTextures())
        {
            gs->processTouchEvents();
            return;
        }
        
        if (m_iLevelToLoad > 0)
        {
            WorldMapToLevel::getInstance()->setLevelToLoad(m_iLevelToLoad);
            gs->m_stateMachine->changeState(WorldMapToLevel::getInstance());
        }
        
        gs->processTouchEvents();

        for (std::vector<TouchEvent>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
        {
            gs->touchToWorld((*i));
            
            switch (i->getTouchType())
            {
                case DOWN:
                    continue;
                case DRAGGED:
                    continue;
                case UP:
                    if (OverlapTester::isPointInRectangle(*gs->m_touchPoint, m_backButton->getMainBounds()))
                    {
                        gs->m_stateMachine->revertToPreviousState();
                    }
                    else
                    {
                        for (std::vector<std::unique_ptr<LevelThumbnail>>::iterator j = m_levelThumbnails.begin(); j != m_levelThumbnails.end(); j++)
                        {
                            if ((*j)->isVisible() && OverlapTester::isPointInRectangle(*gs->m_touchPoint, (*j)->getMainBounds()))
                            {
                                m_iLevelToLoad = (*j)->getLevel();
                            }
                        }
                    }
                    
                    return;
            }
        }
    }
}

void WorldMap::exit(GameScreen* gs)
{
    m_iLevelToLoad = 0;
}

void WorldMap::loadUserSaveData(const char* json)
{
    m_worldLevelCompletions.clear();
    
    rapidjson::Document d;
    d.Parse<0>(json);
    
    loadUserSaveData(d, "world_1");
    loadUserSaveData(d, "world_2");
    loadUserSaveData(d, "world_3");
    loadUserSaveData(d, "world_4");
    loadUserSaveData(d, "world_5");
    
    for (std::vector<std::unique_ptr<LevelThumbnail>>::iterator j = m_levelThumbnails.begin(); j != m_levelThumbnails.end(); j++)
    {
        int worldIndex = (*j)->getWorld() - 1;
        int levelIndex = (*j)->getLevel() - 1;
        
        bool isComplete = m_worldLevelCompletions.at(worldIndex)->m_levelCompletions.at(levelIndex);
        
        bool isPreviousLevelComplete = false;
        if (worldIndex == 0 && levelIndex == 0)
        {
            isPreviousLevelComplete = true;
        }
        else
        {
            int previousLevelIndex = levelIndex == 0 ? 21 : levelIndex - 1;
            int previousWorldIndex = levelIndex == 0 ? worldIndex - 1 : worldIndex;
            
            isPreviousLevelComplete = m_worldLevelCompletions.at(previousWorldIndex)->m_levelCompletions.at(previousLevelIndex);
        }
        
        (*j)->setVisible(isComplete || isPreviousLevelComplete);
        (*j)->setCompleted(isComplete);
    }
}

std::vector<std::unique_ptr<LevelThumbnail>>& WorldMap::getLevelThumbnails()
{
    return m_levelThumbnails;
}

WorldMapPanel* WorldMap::getWorldMapPanel()
{
    return m_panel.get();
}

BackButton* WorldMap::getBackButton()
{
    return m_backButton.get();
}

#pragma mark private

void WorldMap::loadUserSaveData(rapidjson::Document& d, const char * key)
{
    using namespace rapidjson;
    
    if (d.HasMember(key))
    {
        Value& v = d[key];
        assert(v.IsArray());
        
        WorldLevelCompletions* wlc = new WorldLevelCompletions();
        
        for (SizeType i = 0; i < v.Size(); i++)
        {
            bool isComplete = v[i].GetInt();
            wlc->m_levelCompletions.push_back(isComplete);
        }
        
        m_worldLevelCompletions.push_back(std::unique_ptr<WorldLevelCompletions>(wlc));
    }
}

WorldMap::WorldMap() : m_iLevelToLoad(0)
{
    m_panel = std::unique_ptr<WorldMapPanel>(new WorldMapPanel());
    m_backButton = std::unique_ptr<BackButton>(new BackButton());
    
    m_levelThumbnails.push_back(std::unique_ptr<LevelThumbnail>(new LevelThumbnail(1.0f, 7.0f, 1, 1)));
    m_levelThumbnails.push_back(std::unique_ptr<LevelThumbnail>(new LevelThumbnail(4.0f, 7.0f, 1, 2)));
    m_levelThumbnails.push_back(std::unique_ptr<LevelThumbnail>(new LevelThumbnail(7.0f, 7.0f, 1, 3)));
//    m_levelThumbnails.push_back(std::unique_ptr<LevelThumbnail>(new LevelThumbnail(10.0f, 7.0f, 1, 4)));
//    m_levelThumbnails.push_back(std::unique_ptr<LevelThumbnail>(new LevelThumbnail(13.0f, 7.0f, 1, 5)));
//    m_levelThumbnails.push_back(std::unique_ptr<LevelThumbnail>(new LevelThumbnail(1.0f, 5.0f, 1, 6)));
//    m_levelThumbnails.push_back(std::unique_ptr<LevelThumbnail>(new LevelThumbnail(4.0f, 5.0f, 1, 7)));
//    m_levelThumbnails.push_back(std::unique_ptr<LevelThumbnail>(new LevelThumbnail(7.0f, 5.0f, 1, 8)));
//    m_levelThumbnails.push_back(std::unique_ptr<LevelThumbnail>(new LevelThumbnail(10.0f, 5.0f, 1, 9)));
//    m_levelThumbnails.push_back(std::unique_ptr<LevelThumbnail>(new LevelThumbnail(13.0f, 5.0f, 1, 10)));
}
