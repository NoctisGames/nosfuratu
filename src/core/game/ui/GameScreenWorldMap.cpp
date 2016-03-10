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
    gs->m_renderer->init(RENDERER_TYPE_WORLD_MAP);
    
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
            gs->m_renderer->renderWorldMapScreenUi(m_backButton.get());
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
                    else if (gs->m_touchPoint->getY() < (CAM_HEIGHT * 2 / 3))
                    {
                        if (gs->m_touchPoint->getX() > 0.25f + CAM_WIDTH * 9 / 11)
                        {
                            m_iLevelToLoad = 10;
                        }
                        else if (gs->m_touchPoint->getX() > 0.25f + CAM_WIDTH * 8 / 11)
                        {
                            m_iLevelToLoad = 9;
                        }
                        else if (gs->m_touchPoint->getX() > 0.25f + CAM_WIDTH * 7 / 11)
                        {
                            m_iLevelToLoad = 8;
                        }
                        else if (gs->m_touchPoint->getX() > 0.25f + CAM_WIDTH * 6 / 11)
                        {
                            m_iLevelToLoad = 7;
                        }
                        else if (gs->m_touchPoint->getX() > 0.25f + CAM_WIDTH * 5 / 11)
                        {
                            m_iLevelToLoad = 6;
                        }
                        else if (gs->m_touchPoint->getX() > 0.25f + CAM_WIDTH * 4 / 11)
                        {
                            m_iLevelToLoad = 5;
                        }
                        else if (gs->m_touchPoint->getX() > 0.25f + CAM_WIDTH * 3 / 11)
                        {
                            m_iLevelToLoad = 4;
                        }
                        else if (gs->m_touchPoint->getX() > 0.25f + CAM_WIDTH * 2 / 11)
                        {
                            m_iLevelToLoad = 3;
                        }
                        else if (gs->m_touchPoint->getX() > 0.25f + CAM_WIDTH * 1 / 11)
                        {
                            m_iLevelToLoad = 2;
                        }
                        else
                        {
                            m_iLevelToLoad = 1;
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

WorldMapPanel* WorldMap::getWorldMapPanel()
{
    return m_panel.get();
}

BackButton* WorldMap::getBackButton()
{
    return m_backButton.get();
}

WorldMap::WorldMap() : m_iLevelToLoad(0)
{
    m_panel = std::unique_ptr<WorldMapPanel>(new WorldMapPanel());
    m_backButton = std::unique_ptr<BackButton>(new BackButton());
}
