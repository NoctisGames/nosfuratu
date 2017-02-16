//
//  GameScreenOpeningCutscene.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 8/2/16.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#include "GameScreenOpeningCutscene.h"

#include "GameScreen.h"
#include "GameScreenTitle.h"
#include "GameScreenTransitions.h"
#include "CutscenePanel.h"
#include "Game.h"
#include "EntityUtils.h"
#include "BatPanel.h"
#include "GameScreenWorldMap.h"
#include "ScreenInputManager.h"
#include "KeyboardInputManager.h"
#include "GamePadInputManager.h"
#include "KeyboardEvent.h"
#include "GamePadEvent.h"

OpeningCutscene * OpeningCutscene::getInstance()
{
    static OpeningCutscene *instance = new OpeningCutscene();
    
    return instance;
}

void OpeningCutscene::enter(GameScreen* gs)
{
    gs->m_stateMachine->setPreviousState(Title::getInstance());
    
    initRenderer(gs);
    
    if (m_needsReset)
    {
        EntityUtils::cleanUpVectorOfPointers(m_cutscenePanels);
        
        m_cutscenePanels.push_back(new CutscenePanelOpeningOne());
        
        m_currentPanelIndex = 0;
    }
}

void OpeningCutscene::initRenderer(GameScreen* gs)
{
    gs->m_renderer->init(RENDERER_TYPE_WORLD_1_CUTSCENE);
}

void OpeningCutscene::execute(GameScreen* gs)
{
    if (gs->m_isRequestingRender)
    {
        gs->m_renderer->beginFrame(gs->m_fDeltaTime);
        
        gs->m_renderer->renderCutscene(m_cutscenePanels);
        
        if (gs->m_renderer->isLoadingAdditionalTextures())
        {
            gs->m_renderer->renderLoading();
        }
        
        gs->m_renderer->renderToScreen();
        
        gs->m_renderer->endFrame();
    }
    else
    {
        if (m_isRequestingNextState)
        {
            gs->m_stateMachine->changeState(OpeningCutsceneToWorldMap::getInstance());
            return;
        }
        
        if (m_cutscenePanels.size() > 0)
        {
            CutscenePanel* cPanel = m_cutscenePanels.at(m_currentPanelIndex);
            EntityUtils::updateAndClean(m_cutscenePanels, gs->m_fDeltaTime);
            
            if (cPanel->getColor().alpha >= 1
                && m_cutscenePanels.size() > 1)
            {
                CutscenePanel* dPanel = m_cutscenePanels.at(m_currentPanelIndex == 0 ? 1 : 0);
                dPanel->onCleanUp();
                delete dPanel;
                
                if (m_currentPanelIndex == 0)
                {
                    m_cutscenePanels.pop_back();
                }
                else
                {
                    m_cutscenePanels.erase(m_cutscenePanels.begin());
                    m_currentPanelIndex = 0;
                }
            }
            
            if (cPanel->isReadyForNextPanel())
            {
                CutscenePanel* nPanel = cPanel->getNextPanel();
                if (nPanel)
                {
                    if (nPanel->getType() == CutscenePanelType_Opening_Six)
                    {
                        m_currentPanelIndex = 1;
                        m_cutscenePanels.push_back(nPanel);
                    }
                    else
                    {
                        m_currentPanelIndex = 0;
                        m_cutscenePanels.insert(m_cutscenePanels.begin(), nPanel);
                    }
                    
                    return;
                }
                else
                {
                    gs->m_iRequestedAction = REQUESTED_ACTION_SET_CUTSCENE_VIEWED * 1000;
                    gs->m_iRequestedAction += FLAG_CUTSCENE_VIEWED_OPENING;
                    
                    m_isRequestingNextState = true;
                }
            }
        }
        
        if (FlagUtil::isFlagSet(WorldMap::getInstance()->getViewedCutsceneFlag(), FLAG_CUTSCENE_VIEWED_OPENING))
        {
            for (std::vector<KeyboardEvent *>::iterator i = KEYBOARD_INPUT_MANAGER->getEvents().begin(); i != KEYBOARD_INPUT_MANAGER->getEvents().end(); i++)
            {
                switch ((*i)->getType())
                {
                    case KeyboardEventType_W:
                    case KeyboardEventType_BACK:
                    case KeyboardEventType_SPACE:
                    case KeyboardEventType_ENTER:
                        if ((*i)->isUp())
                        {
                            m_isRequestingNextState = true;
                            return;
                        }
                        continue;
                    default:
                        break;
                }
            }
            
            for (std::vector<GamePadEvent *>::iterator i = GAME_PAD_INPUT_MANAGER->getEvents().begin(); i != GAME_PAD_INPUT_MANAGER->getEvents().end(); i++)
            {
                switch ((*i)->getType())
                {
                    case GamePadEventType_A_BUTTON:
                    case GamePadEventType_B_BUTTON:
                    case GamePadEventType_BACK_BUTTON:
                    case GamePadEventType_START_BUTTON:
                        if ((*i)->isButtonPressed())
                        {
                            m_isRequestingNextState = true;
                            return;
                        }
                        continue;
                    default:
                        break;
                }
            }
            
            for (std::vector<ScreenEvent *>::iterator i = SCREEN_INPUT_MANAGER->getEvents().begin(); i != SCREEN_INPUT_MANAGER->getEvents().end(); i++)
            {
                switch ((*i)->getType())
                {
                    case ScreenEventType_UP:
                        m_isRequestingNextState = true;
                        return;
                    default:
                        continue;
                }
            }
        }
    }
}

void OpeningCutscene::exit(GameScreen* gs)
{
    m_currentPanelIndex = 0;
    m_isRequestingNextState = false;
    m_needsReset = true;
}

std::vector<CutscenePanel*>& OpeningCutscene::getCutscenePanels()
{
    return m_cutscenePanels;
}

#pragma mark - private

CutscenePanelType OpeningCutscene::cutscenePanelTypeForIndex(int index)
{
    switch (index) {
        case 0:
            return CutscenePanelType_Opening_One;
        case 1:
            return CutscenePanelType_Opening_Two;
        case 2:
            return CutscenePanelType_Opening_Three;
        case 3:
            return CutscenePanelType_Opening_Four;
        case 4:
            return CutscenePanelType_Opening_Five;
        case 5:
            return CutscenePanelType_Opening_Six;
        case 6:
            return CutscenePanelType_Opening_Seven;
        default:
            return CutscenePanelType_Opening_None;
    }
}

OpeningCutscene::OpeningCutscene() : m_currentPanelIndex(0), m_isRequestingNextState(false), m_needsReset(false)
{
    m_cutscenePanels.push_back(new CutscenePanelOpeningOne());
}

RTTI_IMPL(OpeningCutscene, GameScreenState);
