//
//  GameScreenWorldMap.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/10/16.
//  Copyright © 2016 Noctis Games. All rights reserved.
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
    gs->m_renderer->init(RENDERER_TYPE_MENU);
    gs->m_iNumFramesToDiscard = 1;
}

void WorldMap::execute(GameScreen* gs)
{
    if (gs->m_isRequestingRender)
    {
        gs->m_renderer->beginFrame();
        
        gs->m_renderer->renderWorldMapScreenBackground();
        
        if (m_iLevelToLoad > 0)
        {
            gs->m_renderer->renderLoadingTextOnWorldMapScreen();
        }
        else
        {
            gs->m_renderer->renderWorldMapScreenUi(*m_backButton);
        }
        
        gs->m_renderer->renderToScreen();
        
        gs->m_renderer->endFrame();
    }
    else
    {
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
                    if (OverlapTester::isPointInRectangle(*gs->m_touchPoint, m_backButton->getBounds()))
                    {
                        gs->m_stateMachine->revertToPreviousState();
                    }
                    else if (gs->m_touchPoint->getY() < (CAM_HEIGHT * 2 / 3))
                    {
                        if (gs->m_touchPoint->getX() > CAM_WIDTH * 2 / 3)
                        {
                            m_iLevelToLoad = 3;
                        }
                        else if (gs->m_touchPoint->getX() > CAM_WIDTH / 3)
                        {
                            m_iLevelToLoad = 2;
                        }
                        else if (gs->m_touchPoint->getX() > 0)
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

BackButton& WorldMap::getBackButton()
{
    return *m_backButton;
}

WorldMap::WorldMap() : m_iLevelToLoad(0)
{
    m_backButton = std::unique_ptr<BackButton>(new BackButton());
}
