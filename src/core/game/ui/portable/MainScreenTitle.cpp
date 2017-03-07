//
//  MainScreenTitle.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/10/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "MainScreenTitle.h"

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
#include "TouchConverter.h"
#include "MainRenderer.h"
#include "ScreenEvent.h"

/// Title Screen ///

Title * Title::getInstance()
{
    static Title *instance = new Title();
    
    return instance;
}

void Title::enter(MainScreen* ms)
{
    ms->m_stateMachine.setPreviousState(nullptr);
    
    SOUND_MANAGER->addMusicIdToPlayQueue(MUSIC_LOAD_TITLE_LOOP);
    SOUND_MANAGER->addMusicIdToPlayQueue(MUSIC_PLAY_LOOP);
    
    ms->m_iRequestedAction = REQUESTED_ACTION_GET_SAVE_DATA;
}

void Title::execute(MainScreen* ms)
{
    if (ms->m_isRequestingRender)
    {
        ms->m_renderer->beginFrame();
        
        ms->m_renderer->renderTitleScreenBackground(m_panel);
        
        if (ms->m_renderer->isLoadingData())
        {
            ms->m_renderer->renderLoading(ms->m_fDeltaTime * ms->m_iNumInternalUpdates);
        }
        else
        {
            ms->m_renderer->renderTitleScreenUi(m_levelEditorButton);
        }
        
        ms->m_renderer->renderToScreen();
        
        ms->m_renderer->endFrame();
    }
    else
    {
        m_panel->update(ms->m_fDeltaTime);
        
        if (m_isRequestingNextState)
        {
            bool isOpeningCutsceneViewed = FlagUtil::isFlagSet(WorldMap::getInstance()->getViewedCutsceneFlag(), FLAG_CUTSCENE_VIEWED_OPENING);
#if NG_LEVEL_EDITOR
            isOpeningCutsceneViewed = true;
#endif
            if (isOpeningCutsceneViewed)
            {
                ms->m_stateMachine.changeState(TitleToWorldMap::getInstance());
            }
            else
            {
                ms->m_stateMachine.changeState(TitleToOpeningCutscene::getInstance());
            }
        }
        else if (m_isRequestingLevelEditor)
        {
            ms->m_stateMachine.changeState(TitleToLevelEditor::getInstance());
        }
        
		bool isDisplayingLevelEditorButtons = false;
#if NG_LEVEL_EDITOR
		isDisplayingLevelEditorButtons = true;
#endif
        for (std::vector<KeyboardEvent *>::iterator i = KEYBOARD_INPUT_MANAGER->getEvents().begin(); i != KEYBOARD_INPUT_MANAGER->getEvents().end(); i++)
        {
            switch ((*i)->getType())
            {
                case KeyboardEventType_SPACE:
                case KeyboardEventType_ENTER:
                    m_isRequestingNextState = true;
                    return;
                default:
                    continue;
            }
        }
        
        for (std::vector<GamePadEvent *>::iterator i = GAME_PAD_INPUT_MANAGER->getEvents().begin(); i != GAME_PAD_INPUT_MANAGER->getEvents().end(); i++)
        {
            switch ((*i)->getType())
            {
                case GamePadEventType_A_BUTTON:
                case GamePadEventType_START_BUTTON:
                    m_isRequestingNextState = true;
                    return;
                default:
                    continue;
            }
        }
        
		for (std::vector<ScreenEvent *>::iterator i = SCREEN_INPUT_MANAGER->getEvents().begin(); i != SCREEN_INPUT_MANAGER->getEvents().end(); i++)
        {
            Vector2D& touchPoint = TOUCH_CONVERTER->touchToWorld(*(*i));
            
            switch ((*i)->getType())
            {
                case ScreenEventType_DOWN:
                    continue;
                case ScreenEventType_DRAGGED:
                    continue;
                case ScreenEventType_UP:
					if (isDisplayingLevelEditorButtons
                        && m_levelEditorButton->handleClick(touchPoint))
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

void Title::exit(MainScreen* ms)
{
    m_isRequestingNextState = false;
    m_isRequestingLevelEditor = false;
}

void Title::initRenderer(MainScreen* ms)
{
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_MAP);
    
    ms->m_renderer->unload(RENDERER_TYPE_LEVEL_EDITOR);
    
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_1_CUTSCENE);
    
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_1);
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_1_MID_BOSS);
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_1_END_BOSS);
    
    ms->m_renderer->load(RENDERER_TYPE_TITLE);
}

TitlePanel* Title::getTitlePanel()
{
    return m_panel;
}

GameButton* Title::getLevelEditorButton()
{
    return m_levelEditorButton;
}

Title::Title() : MainScreenState(),
m_panel(new TitlePanel()),
m_levelEditorButton(GameButton::create(GameButtonType_LevelEditor)),
m_isRequestingNextState(false),
m_isRequestingLevelEditor(false)
{
    // Empty
}

Title::~Title()
{
    delete m_panel;
    delete m_levelEditorButton;
}

RTTI_IMPL(Title, MainScreenState);
