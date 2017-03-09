//
//  MainScreenComingSoon.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 10/6/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "MainScreenComingSoon.h"

#include "MainScreen.h"

#include "State.h"
#include "MainScreen.h"
#include "EntityUtils.h"
#include "Vector2D.h"
#include "Game.h"
#include "MainScreenTransitions.h"
#include "MainScreenWorldMap.h"
#include "MainScreenOpeningCutscene.h"
#include "ScreenInputManager.h"
#include "KeyboardInputManager.h"
#include "GamePadInputManager.h"
#include "KeyboardEvent.h"
#include "GamePadEvent.h"
#include "MainRenderer.h"
#include "ScreenEvent.h"
#include "NGAudioEngine.h"

/// ComingSoon Screen ///

ComingSoon * ComingSoon::getInstance()
{
    static ComingSoon *instance = new ComingSoon();
    
    return instance;
}

void ComingSoon::enter(MainScreen* ms)
{
    ms->m_stateMachine.setPreviousState(WorldMap::getInstance());
    
    initRenderer(ms);
}

void ComingSoon::execute(MainScreen* ms)
{
    if (ms->m_isRequestingRender)
    {
        ms->m_renderer->beginFrame();
        
        ms->m_renderer->renderComingSoonScreenBackground();
        
        if (ms->m_renderer->isLoadingData())
        {
            ms->m_renderer->renderLoading(ms->m_fDeltaTime * ms->m_iNumInternalUpdates);
        }
        
        ms->m_renderer->renderToScreen();
        
        ms->m_renderer->endFrame();
    }
    else
    {
        if (m_isRequestingNextState)
        {
            ms->m_stateMachine.revertToPreviousState();
            
            return;
        }
        
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
                    continue;
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
                    continue;
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

void ComingSoon::exit(MainScreen* ms)
{
    m_isRequestingNextState = false;
    
    NG_AUDIO_ENGINE->loadMusic("level_select_bgm");
    NG_AUDIO_ENGINE->playMusic(true);
}

void ComingSoon::initRenderer(MainScreen* ms)
{
    ms->m_renderer->load(RENDERER_TYPE_WORLD_1);
}

ComingSoon::ComingSoon() : m_isRequestingNextState(false)
{
    // Empty
}

RTTI_IMPL(ComingSoon, MainScreenState);
