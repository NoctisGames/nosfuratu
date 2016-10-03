//
//  GameScreenTitle.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/10/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "GameScreenTitle.h"
#include "State.h"
#include "GameScreen.h"
#include "EntityUtils.h"
#include "Vector2D.h"
#include "Game.h"
#include "GameScreenTransitions.h"
#include "GameScreenWorldMap.h"
#include "GameScreenOpeningCutscene.h"

/// Title Screen ///

Title * Title::getInstance()
{
    static Title *instance = new Title();
    
    return instance;
}

void Title::enter(GameScreen* gs)
{
    gs->m_stateMachine->setPreviousState(nullptr);
    
    gs->m_renderer->unload(RENDERER_TYPE_WORLD_MAP);
    gs->m_renderer->unload(RENDERER_TYPE_LEVEL_EDITOR);
    
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
    
    gs->m_renderer->init(RENDERER_TYPE_TITLE);
    
    Assets::getInstance()->setMusicId(MUSIC_PLAY_TITLE_LOOP);
    
    gs->m_iRequestedAction = REQUESTED_ACTION_GET_SAVE_DATA;
}

void Title::execute(GameScreen* gs)
{
    if (gs->m_isRequestingRender)
    {
        gs->m_renderer->beginFrame(gs->m_fDeltaTime);
        
        gs->m_renderer->renderTitleScreenBackground(m_panel.get());
        
        if (gs->m_renderer->isLoadingAdditionalTextures())
        {
            gs->m_renderer->renderLoading();
        }
        else
        {
            gs->m_renderer->renderTitleScreenUi(m_levelEditorButton.get());
        }
        
        gs->m_renderer->renderToScreen();
        
        gs->m_renderer->endFrame();
    }
    else
    {
        m_panel->update(gs->m_fDeltaTime);
        
        m_levelEditorButton->update(gs->m_fDeltaTime);
        
        if (m_isRequestingNextState)
        {
            if (FlagUtil::isFlagSet(WorldMap::getInstance()->getViewedCutsceneFlag(), FLAG_CUTSCENE_VIEWED_OPENING))
            {
                gs->m_stateMachine->changeState(TitleToWorldMap::getInstance());
            }
            else
            {
                gs->m_stateMachine->changeState(TitleToOpeningCutscene::getInstance());
            }
        }
        else if (m_isRequestingLevelEditor)
        {
            gs->m_stateMachine->changeState(TitleToLevelEditor::getInstance());
        }
        
		bool isDisplayingLevelEditorButton = false;
#if DEBUG || _DEBUG
		isDisplayingLevelEditorButton = true;
#endif
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
					if (isDisplayingLevelEditorButton
                        && m_levelEditorButton->handleClick(*gs->m_touchPoint))
                    {
                        m_isRequestingLevelEditor = true;
                    }
                    else
                    {
                        m_isRequestingNextState = true;
                    }
                    
                    return;
            }
        }
    }
}

void Title::exit(GameScreen* gs)
{
    m_isRequestingNextState = false;
    m_isRequestingLevelEditor = false;
}

TitlePanel* Title::getTitlePanel()
{
    return m_panel.get();
}

GameButton* Title::getLevelEditorButton()
{
    return m_levelEditorButton.get();
}

Title::Title() : m_isRequestingNextState(false), m_isRequestingLevelEditor(false)
{
    m_panel = std::unique_ptr<TitlePanel>(new TitlePanel());
    m_levelEditorButton = std::unique_ptr<GameButton>(GameButton::create(GameButtonType_LevelEditor));
}
