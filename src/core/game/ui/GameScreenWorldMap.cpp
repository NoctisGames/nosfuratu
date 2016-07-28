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

/// World Map ///

WorldMap * WorldMap::getInstance()
{
    static WorldMap *instance = new WorldMap();
    
    return instance;
}

void WorldMap::enter(GameScreen* gs)
{
	gs->m_stateMachine->setPreviousState(Title::getInstance());
    
    gs->m_renderer->unload(RENDERER_TYPE_WORLD_1);
    gs->m_renderer->unload(RENDERER_TYPE_WORLD_1_MID_BOSS);
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
    
    gs->m_renderer->init(RENDERER_TYPE_WORLD_MAP);
    
    gs->m_iRequestedAction = REQUESTED_ACTION_GET_LEVEL_STATS;
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
//            gs->m_renderer->renderWorldMapScreenUi(m_levelThumbnails, m_menu.get(), m_backButton.get());
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
        
        if (m_isReadyForTransition)
        {
            gs->m_stateMachine->changeState(WorldMapToLevel::getInstance());
			return;
        }
        
        gs->processTouchEvents();

        for (std::vector<TouchEvent *>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
        {
            gs->touchToWorld(*(*i));
            
            switch ((*i)->getTouchType())
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
                                int worldToLoad = (*j)->getWorld();
                                int levelToLoad = (*j)->getLevel();
                                int goldenCarrotsFlag = (*j)->getGoldenCarrotsFlag();
                                
                                WorldMapToLevel::getInstance()->setWorldToLoad(worldToLoad);
                                WorldMapToLevel::getInstance()->setLevelToLoad(levelToLoad);
                                
                                if (m_menu->getWorld() == worldToLoad
                                    && m_menu->getLevel() == levelToLoad)
                                {
                                    m_isReadyForTransition = true;
									return;
                                }
                                
                                // Temp
                                m_isReadyForTransition = true;
                                
                                m_menu->setLevelStats(worldToLoad, levelToLoad, goldenCarrotsFlag);
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
    m_isReadyForTransition = false;
}

void WorldMap::loadUserSaveData(const char* json)
{
    m_worldLevelStats.clear();
    
    m_menu->setLevelStats(-1, -1, 0);
    
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
        
        int levelStats = m_worldLevelStats.at(worldIndex)->m_levelStats.at(levelIndex);
        
        (*j)->setLevelStats(levelStats);
        
        int previousLevelStats = false;
        if (worldIndex == 0 && levelIndex == 0)
        {
            previousLevelStats = 1;
        }
        else
        {
            int previousLevelIndex = levelIndex == 0 ? 21 : levelIndex - 1;
            int previousWorldIndex = levelIndex == 0 ? worldIndex - 1 : worldIndex;
            
            previousLevelStats = m_worldLevelStats.at(previousWorldIndex)->m_levelStats.at(previousLevelIndex);
        }
        
        (*j)->setVisible(previousLevelStats > 0);
        (*j)->setCompleted(levelStats > 0);
        
        if ((*j)->isVisible())
        {
            if ((*j)->isCompleted())
            {
                int nextLevelIndex = levelIndex == 21 ? 0 : levelIndex + 1;
                int nextWorldIndex = levelIndex == 21 ? worldIndex + 1 : worldIndex;
                
                int nextLevelStats = m_worldLevelStats.at(nextWorldIndex)->m_levelStats.at(nextLevelIndex);
                
                if (nextLevelStats == 0)
                {
                    (*j)->animate(0);
                }
            }
            else
            {
                (*j)->animate(0.60f);
            }
        }
        
        // Temporary
        (*j)->setVisible(true);
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

WorldMapMenu* WorldMap::getWorldMapMenu()
{
    return m_menu.get();
}

BackButton* WorldMap::getBackButton()
{
    return m_backButton.get();
}

float WorldMap::getCamPosY()
{
    return m_fCamPosY;
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
            int levelStats = v[i].GetInt();
            wlc->m_levelStats.push_back(levelStats);
        }
        
        m_worldLevelStats.push_back(std::unique_ptr<WorldLevelCompletions>(wlc));
    }
}

WorldMap::WorldMap() : m_fCamPosY(0), m_isReadyForTransition(false)
{
    m_panel = std::unique_ptr<WorldMapPanel>(new WorldMapPanel());
    m_menu = std::unique_ptr<WorldMapMenu>(new WorldMapMenu());
    m_backButton = std::unique_ptr<BackButton>(new BackButton());
    
    float pW = m_panel->getWidth();
    float pH = m_panel->getHeight();
    
    m_levelThumbnails.push_back(std::unique_ptr<LevelThumbnail>(new LevelThumbnail(pW * 0.19117647058824f, pH * 0.56209150326797f, 1, 1)));
    m_levelThumbnails.push_back(std::unique_ptr<LevelThumbnail>(new LevelThumbnail(pW * 0.30882352941176f, pH * 0.56209150326797f, 1, 2)));
    m_levelThumbnails.push_back(std::unique_ptr<LevelThumbnail>(new LevelThumbnail(pW * 0.42647058823529f, pH * 0.56209150326797f, 1, 3)));
    m_levelThumbnails.push_back(std::unique_ptr<LevelThumbnail>(new LevelThumbnail(pW * 0.54411764705882f, pH * 0.56209150326797f, 1, 4)));
    m_levelThumbnails.push_back(std::unique_ptr<LevelThumbnail>(new LevelThumbnail(pW * 0.66176470588235f, pH * 0.56209150326797f, 1, 5)));
    m_levelThumbnails.push_back(std::unique_ptr<LevelThumbnail>(new LevelThumbnail(pW * 0.77941176470588f, pH * 0.56209150326797f, 1, 6)));
    m_levelThumbnails.push_back(std::unique_ptr<LevelThumbnail>(new LevelThumbnail(pW * 0.89705882352941f, pH * 0.45751633986928f, 1, 7)));
    m_levelThumbnails.push_back(std::unique_ptr<LevelThumbnail>(new LevelThumbnail(pW * 0.77941176470588f, pH * 0.35294117647059f, 1, 8)));
    m_levelThumbnails.push_back(std::unique_ptr<LevelThumbnail>(new LevelThumbnail(pW * 0.66176470588235f, pH * 0.35294117647059f, 1, 9)));
    m_levelThumbnails.push_back(std::unique_ptr<LevelThumbnail>(new LevelThumbnail(pW * 0.54411764705882f, pH * 0.35294117647059f, 1, 10)));
    m_levelThumbnails.push_back(std::unique_ptr<LevelThumbnail>(new LevelThumbnail(pW * 0.42647058823529f, pH * 0.35294117647059f, 1, 11)));
    m_levelThumbnails.push_back(std::unique_ptr<LevelThumbnail>(new LevelThumbnail(pW * 0.30882352941176f, pH * 0.35294117647059f, 1, 12)));
    m_levelThumbnails.push_back(std::unique_ptr<LevelThumbnail>(new LevelThumbnail(pW * 0.19117647058824f, pH * 0.35294117647059f, 1, 13)));
    m_levelThumbnails.push_back(std::unique_ptr<LevelThumbnail>(new LevelThumbnail(pW * 0.07352941176471f, pH * 0.2483660130719f, 1, 14)));
    m_levelThumbnails.push_back(std::unique_ptr<LevelThumbnail>(new LevelThumbnail(pW * 0.19117647058824f, pH * 0.1437908496732f, 1, 15)));
    m_levelThumbnails.push_back(std::unique_ptr<LevelThumbnail>(new LevelThumbnail(pW * 0.30882352941176f, pH * 0.1437908496732f, 1, 16)));
    m_levelThumbnails.push_back(std::unique_ptr<LevelThumbnail>(new LevelThumbnail(pW * 0.42647058823529f, pH * 0.1437908496732f, 1, 17)));
    m_levelThumbnails.push_back(std::unique_ptr<LevelThumbnail>(new LevelThumbnail(pW * 0.54411764705882f, pH * 0.1437908496732f, 1, 18)));
    m_levelThumbnails.push_back(std::unique_ptr<LevelThumbnail>(new LevelThumbnail(pW * 0.66176470588235f, pH * 0.1437908496732f, 1, 19)));
    m_levelThumbnails.push_back(std::unique_ptr<LevelThumbnail>(new LevelThumbnail(pW * 0.77941176470588f, pH * 0.1437908496732f, 1, 20)));
    m_levelThumbnails.push_back(std::unique_ptr<LevelThumbnail>(new LevelThumbnail(pW * 0.89705882352941f, pH * 0.1437908496732f, 1, 21)));
}
