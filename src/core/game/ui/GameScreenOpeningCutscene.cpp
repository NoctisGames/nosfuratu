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

OpeningCutscene * OpeningCutscene::getInstance()
{
    static OpeningCutscene *instance = new OpeningCutscene();
    
    return instance;
}

void OpeningCutscene::enter(GameScreen* gs)
{
    gs->m_stateMachine->setPreviousState(Title::getInstance());
    
    gs->m_renderer->init(RENDERER_TYPE_WORLD_1_CUTSCENE);
}

void OpeningCutscene::execute(GameScreen* gs)
{
    if (gs->m_isRequestingRender)
    {
        gs->m_renderer->beginFrame(gs->m_fDeltaTime);
        
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
            //gs->m_stateMachine->changeState(WorldMapToLevel::getInstance());
            return;
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
    
}

#pragma mark - private

OpeningCutscene::OpeningCutscene() : m_fStateTime(0), m_fSkipTime(0), m_isRequestingNextState(false), m_isSkipping(false)
{
    
}