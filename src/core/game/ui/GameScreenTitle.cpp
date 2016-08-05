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
            gs->m_renderer->renderTitleScreenUi(m_levelEditorButton.get(), m_isDisplayingLevelEditorButton);
        }
        
        gs->m_renderer->renderToScreen();
        
        gs->m_renderer->endFrame();
    }
    else
    {
        m_panel->update(gs->m_fDeltaTime);
        
        if (m_isRequestingNextState)
        {
            gs->m_stateMachine->changeState(TitleToWorldMap::getInstance());
        }
        else if (m_isRequestingLevelEditor)
        {
            gs->m_stateMachine->changeState(TitleToLevelEditor::getInstance());
        }
        
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
                    if (m_isDisplayingLevelEditorButton
						&& OverlapTester::isPointInRectangle(*gs->m_touchPoint, m_levelEditorButton->getMainBounds()))
                    {
                        m_isRequestingLevelEditor = true;
                    }
                    else if (gs->m_touchPoint->getY() < (CAM_HEIGHT * 2 / 3))
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

void Title::setIsDisplayingLevelEditorButton(bool isDisplayingLevelEditorButton)
{
	m_isDisplayingLevelEditorButton = isDisplayingLevelEditorButton;
}

Title::Title() : m_isRequestingNextState(false), m_isRequestingLevelEditor(false), m_isDisplayingLevelEditorButton(false)
{
    m_panel = std::unique_ptr<TitlePanel>(new TitlePanel());
    m_levelEditorButton = std::unique_ptr<GameButton>(GameButton::create(GameButtonType_LevelEditor));
}
