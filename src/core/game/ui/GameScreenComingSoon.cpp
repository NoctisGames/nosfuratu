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
        
		for (std::vector<TouchEvent *>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
        {
            switch ((*i)->getTouchType())
            {
                case DOWN:
                    continue;
                case DRAGGED:
                    continue;
                case UP:
					m_isRequestingNextState = true;
                    
                    return;
            }
        }
    }
}

void ComingSoon::exit(GameScreen* gs)
{
    m_isRequestingNextState = false;
    
    Assets::getInstance()->setMusicId(MUSIC_PLAY_LEVEL_SELECT_LOOP);
}

ComingSoon::ComingSoon() : m_isRequestingNextState(false)
{
    // Empty
}
