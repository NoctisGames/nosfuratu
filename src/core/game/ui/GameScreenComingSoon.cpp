//
//  GameScreenComingSoon.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 10/6/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "GameScreenComingSoon.h"
#include "State.h"
#include "GameScreen.h"
#include "EntityUtils.h"
#include "Vector2D.h"
#include "Game.h"
#include "GameScreenTransitions.h"
#include "GameScreenWorldMap.h"
#include "GameScreenOpeningCutscene.h"
#include "ScreenInputManager.h"
#include "KeyboardInputManager.h"
#include "GamePadInputManager.h"
#include "KeyboardEvent.h"
#include "GamePadEvent.h"

/// ComingSoon Screen ///

ComingSoon * ComingSoon::getInstance()
{
    static ComingSoon *instance = new ComingSoon();
    
    return instance;
}

void ComingSoon::enter(GameScreen* gs)
{
    gs->m_stateMachine->setPreviousState(WorldMap::getInstance());
}

void ComingSoon::initRenderer(GameScreen* gs)
{
    // Empty
}

void ComingSoon::execute(GameScreen* gs)
{
    if (gs->m_isRequestingRender)
    {
        gs->m_renderer->beginFrame(gs->m_fDeltaTime);
        
        gs->m_renderer->renderComingSoonScreenBackground();
        
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
            gs->m_stateMachine->revertToPreviousState();
            
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

void ComingSoon::exit(GameScreen* gs)
{
    m_isRequestingNextState = false;
    
    ASSETS->addMusicIdToPlayQueue(MUSIC_LOAD_LEVEL_SELECT_LOOP);
    ASSETS->addMusicIdToPlayQueue(MUSIC_PLAY_LOOP);
}

ComingSoon::ComingSoon() : m_isRequestingNextState(false)
{
    // Empty
}

RTTI_IMPL(ComingSoon, GameScreenState);
