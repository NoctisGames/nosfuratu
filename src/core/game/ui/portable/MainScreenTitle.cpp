//
//  MainScreenTitle.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/10/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "MainScreenTitle.h"

#include "MainScreen.h"
#include "GameButton.h"
#include "TitlePanel.h"

#include "State.h"
#include "MainScreen.h"
#include "EntityUtils.h"
#include "Vector2D.h"
#include "Game.h"
#include "MainScreenTransitions.h"
#include "MainScreenOpeningCutscene.h"
#include "ScreenInputManager.h"
#include "KeyboardInputManager.h"
#include "GamePadInputManager.h"
#include "KeyboardEvent.h"
#include "GamePadEvent.h"
#include "TouchConverter.h"
#include "MainRenderer.h"
#include "ScreenEvent.h"
#include "FlagUtil.h"
#include "NGAudioEngine.h"
#include "SaveDataKeys.h"
#include "SaveData.h"
#include "StringUtil.h"
#include "MainScreenWorldMap.h"

/// Title Screen ///

Title * Title::getInstance()
{
    static Title *instance = new Title();
    
    return instance;
}

void Title::enter(MainScreen* ms)
{
    ms->m_stateMachine.setPreviousState(nullptr);
    
    NG_AUDIO_ENGINE->loadMusic("title_bgm");
    NG_AUDIO_ENGINE->playMusic(true);
    
    WorldMap::getInstance()->loadSaveData();
    
    m_fStateTime = 0;
    m_fCodeStateTime = 0;
    m_iResetCodeState = 0;
    m_iMapCodeState = 0;
    m_iSwampCodeState =0;
    m_iDmCodeState = 0;
    m_isRequestingNextState = false;
    m_isRequestingLevelEditor = false;
    m_isLevelEditor = false;
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
        m_fStateTime += ms->m_fDeltaTime;
        
        if (m_fStateTime > 13)
        {
            srand (static_cast <unsigned> (time(0)));
            int level = rand() % 8 + 1;
            if (level == 8) { level = 16; }
            if (level == 7) { level = 15; }
            if (level == 6) { level = 14; }
            if (level == 5) { level = 10; }
            if (level == 4) { level = 9; }
            if (level == 3) { level = 6; }
            
            TitleToDemo::getInstance()->setWorldToLoad(1);
            TitleToDemo::getInstance()->setLevelToLoad(level);
            
            ms->m_stateMachine.changeState(TitleToDemo::getInstance());
            
            return;
        }
        
        m_panel->update(ms->m_fDeltaTime);
        
        if (m_isRequestingNextState)
        {
            std::string key = getKeyForViewedCutscenesFlag();
            std::string val = NG_SAVE_DATA->findValue(key);
            int viewedCutsceneFlag = StringUtil::stringToInt(val);
            
            bool isOpeningCutsceneViewed = FlagUtil::isFlagSet(viewedCutsceneFlag, FLAG_CUTSCENE_VIEWED_OPENING);
            
            if (m_isLevelEditor)
            {
                isOpeningCutsceneViewed = true;
            }
            
            if (isOpeningCutsceneViewed)
            {
                ms->m_stateMachine.changeState(TitleToWorldMap::getInstance());
            }
            else
            {
                ms->m_stateMachine.changeState(TitleToOpeningCutscene::getInstance());
            }
            
            return;
        }
        else if (m_isRequestingLevelEditor)
        {
            ms->m_stateMachine.changeState(TitleToLevelEditor::getInstance());
            
            return;
        }
        
        if (m_iResetCodeState > 0
            || m_iMapCodeState > 0
            || m_iSwampCodeState > 0
            || m_iDmCodeState > 0)
        {
            m_fCodeStateTime += ms->m_fDeltaTime;
            if (m_fCodeStateTime > 1)
            {
                m_fCodeStateTime = 0;
                
                m_iResetCodeState = 0;
                m_iMapCodeState = 0;
                m_iSwampCodeState =0;
                m_iDmCodeState = 0;
            }
        }
        
		bool isDisplayingLevelEditorButtons = false;
        
        if (m_isLevelEditor)
        {
            isDisplayingLevelEditorButtons = true;
        }
        
        for (std::vector<KeyboardEvent *>::iterator i = KEYBOARD_INPUT_MANAGER->getEvents().begin(); i != KEYBOARD_INPUT_MANAGER->getEvents().end(); ++i)
        {
            switch ((*i)->getType())
            {
                case KeyboardEventType_W:
                    if (m_iSwampCodeState == 1)
                    {
                        m_iSwampCodeState++;
                    }
                    continue;
                case KeyboardEventType_S:
                    if (m_iSwampCodeState == 0)
                    {
                        m_iSwampCodeState++;
                    }
                    continue;
                case KeyboardEventType_V:
                    if (m_iResetCodeState == 0)
                    {
                        m_iResetCodeState++;
                    }
                    continue;
                case KeyboardEventType_A:
                    if (m_iResetCodeState == 1)
                    {
                        m_iResetCodeState++;
                    }
                    else if (m_iSwampCodeState == 2)
                    {
                        m_iSwampCodeState++;
                    }
                    else if (m_iMapCodeState == 1)
                    {
                        m_iMapCodeState++;
                    }
                    continue;
                case KeyboardEventType_D:
                    if (m_iDmCodeState == 0)
                    {
                        m_iDmCodeState++;
                    }
                    continue;
                case KeyboardEventType_M:
                    if (m_iResetCodeState == 2)
                    {
                        m_iResetCodeState++;
                    }
                    else if (m_iSwampCodeState == 3)
                    {
                        m_iSwampCodeState++;
                    }
                    else if (m_iDmCodeState == 1)
                    {
                        m_iDmCodeState++;
                        
                        std::string key = std::string("NG_DEBUG");
                        std::string storedVal = NG_SAVE_DATA->findValue(key);
                        int isDebug = StringUtil::stringToInt(storedVal);
                        
                        std::string val = StringUtil::toString(isDebug == 1 ? 0 : 1);
                        NG_SAVE_DATA->getKeyValues()[key] = val;

						NG_SAVE_DATA->save();
                        
                        WorldMap::getInstance()->loadSaveData();
                        
                        NG_AUDIO_ENGINE->playSound(SOUND_ABILITY_UNLOCK);
                    }
                    else if (m_iMapCodeState == 0)
                    {
                        m_iMapCodeState++;
                    }
                    continue;
                case KeyboardEventType_P:
                    if (m_iResetCodeState == 3)
                    {
                        m_iResetCodeState++;
                        
                        NG_SAVE_DATA->clear();
                        
                        WorldMap::getInstance()->loadSaveData();
                        
                        NG_AUDIO_ENGINE->playSound(SOUND_LEVEL_COMPLETE);
                    }
                    else if (m_iSwampCodeState == 4)
                    {
                        m_iSwampCodeState++;
                        
                        std::string key = std::string("NG_LEVEL_EDITOR");
                        std::string storedVal = NG_SAVE_DATA->findValue(key);
                        int isLevelEditor = StringUtil::stringToInt(storedVal);
                        
                        isLevelEditor = isLevelEditor == 1 ? 0 : 1;
                        
                        m_isLevelEditor = isLevelEditor == 1;
                        
                        std::string val = StringUtil::toString(isLevelEditor);
                        NG_SAVE_DATA->getKeyValues()[key] = val;
                        
						NG_SAVE_DATA->save();

                        WorldMap::getInstance()->loadSaveData();
                        
                        NG_AUDIO_ENGINE->playSound(SOUND_COMPLETE_TRANSFORM);
                    }
                    else if (m_iMapCodeState == 2)
                    {
                        m_iMapCodeState++;
                        
                        std::string key = std::string("NG_UNLOCK_ALL");
                        std::string storedVal = NG_SAVE_DATA->findValue(key);
                        int isUnlockAll = StringUtil::stringToInt(storedVal);
                        
                        std::string val = StringUtil::toString(isUnlockAll == 1 ? 0 : 1);
                        NG_SAVE_DATA->getKeyValues()[key] = val;
                        
						NG_SAVE_DATA->save();

                        WorldMap::getInstance()->loadSaveData();
                        
                        NG_AUDIO_ENGINE->playSound(SOUND_BOSS_LEVEL_UNLOCK);
                    }
                    continue;
                case KeyboardEventType_SPACE:
                case KeyboardEventType_ENTER:
                    m_isRequestingNextState = true;
                    return;
                default:
                    continue;
            }
        }
        
        for (std::vector<GamePadEvent *>::iterator i = GAME_PAD_INPUT_MANAGER->getEvents().begin(); i != GAME_PAD_INPUT_MANAGER->getEvents().end(); ++i)
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
        
		for (std::vector<ScreenEvent *>::iterator i = SCREEN_INPUT_MANAGER->getEvents().begin(); i != SCREEN_INPUT_MANAGER->getEvents().end(); ++i)
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
    m_fStateTime = 0;
    m_fCodeStateTime = 0;
    m_iResetCodeState = 0;
    m_iMapCodeState = 0;
    m_iSwampCodeState =0;
    m_iDmCodeState = 0;
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
m_fStateTime(0),
m_fCodeStateTime(0),
m_iResetCodeState(0),
m_iMapCodeState(0),
m_iSwampCodeState(0),
m_iDmCodeState(0),
m_isRequestingNextState(false),
m_isRequestingLevelEditor(false),
m_isLevelEditor(false)
{
    // Empty
}

Title::~Title()
{
    delete m_panel;
    delete m_levelEditorButton;
}

RTTI_IMPL(Title, MainScreenState);
