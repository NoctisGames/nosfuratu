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

OpeningCutscene * OpeningCutscene::getInstance()
{
    static OpeningCutscene *instance = new OpeningCutscene();
    
    return instance;
}

void OpeningCutscene::enter(GameScreen* gs)
{
    gs->m_stateMachine->setPreviousState(Title::getInstance());
    
    gs->m_renderer->init(RENDERER_TYPE_WORLD_1_CUTSCENE);
    
    m_cutscenePanels.clear();
    
    m_cutscenePanels.push_back(CutscenePanel::create(CutscenePanelType_Opening_One));
    
    m_currentPanelIndex = 0;
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
        
        EntityUtils::updateAndClean(m_cutscenePanels, gs->m_fDeltaTime);
        
        bool isReadyForNextPanel = m_cutscenePanels.size() == 0;
        if (isReadyForNextPanel)
        {
            m_currentPanelIndex++;
            
            CutscenePanelType cutscenePanelType = cutscenePanelTypeForIndex(m_currentPanelIndex);
            if (cutscenePanelType == CutscenePanelType_Opening_None)
            {
                gs->m_iRequestedAction = REQUESTED_ACTION_SET_CUTSCENE_VIEWED * 1000;
                gs->m_iRequestedAction += FLAG_CUTSCENE_VIEWED_OPENING;
                
                m_isRequestingNextState = true;
                
                return;
            }
            
            m_cutscenePanels.push_back(CutscenePanel::create(cutscenePanelType));
        }
        
        if (m_isSkipping)
        {
            m_fSkipTime += gs->m_fDeltaTime;
            
            if (m_fSkipTime > 2)
            {
                m_isRequestingNextState = true;
            }
        }
        
        for (std::vector<TouchEvent *>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
        {
            gs->touchToWorld(*(*i));
            
            switch ((*i)->getTouchType())
            {
                case DOWN:
                    m_isSkipping = true;
                    m_fSkipTime = 0;
                    continue;
                case DRAGGED:
                    continue;
                case UP:
                    m_isSkipping = false;
                    m_fSkipTime = 0;
                    
                    return;
            }
        }
    }
}

void OpeningCutscene::exit(GameScreen* gs)
{
    m_currentPanelIndex = 0;
    m_fSkipTime = 0;
    m_isRequestingNextState = false;
    m_isSkipping = false;
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

OpeningCutscene::OpeningCutscene() : m_currentPanelIndex(0), m_fSkipTime(0), m_isRequestingNextState(false), m_isSkipping(false)
{
    // Empty
}
