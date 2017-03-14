//
//  MainScreenTransitions.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/10/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "MainScreenTransitions.h"

#include "MainScreen.h"
#include "MainScreenLevels.h"
#include "Game.h"

#include "State.h"
#include "EntityUtils.h"
#include "Vector2D.h"
#include "MainScreenLevelEditor.h"
#include "GameButton.h"
#include "MainScreenTitle.h"
#include "MainScreenWorldMap.h"
#include "MainScreenOpeningCutscene.h"
#include "MainScreenComingSoon.h"
#include "BatPanel.h"
#include "NGAudioEngine.h"
#include "MainRenderer.h"

/// Title to Demo Transition ///

TitleToDemo * TitleToDemo::getInstance()
{
    static TitleToDemo *instance = new TitleToDemo();
    
    return instance;
}

void TitleToDemo::enter(MainScreen* ms)
{
    m_fTransitionStateTime = 0;
    
    m_levelState = LevelUtil::getInstanceForWorldAndLevel(m_iWorldToLoad, m_iLevelToLoad);
    
    m_levelState->launchInDemoMode(LevelUtil::getUserDemoActionsForWorldAndLevel(m_iWorldToLoad, m_iLevelToLoad));
    
    m_levelState->enter(ms);
}

void TitleToDemo::execute(MainScreen* ms)
{
    if (ms->m_isRequestingRender)
    {
        ms->m_renderer->beginFrame();
        
        ms->m_renderer->renderTitleScreenBackground(Title::getInstance()->getTitlePanel());
        
        ms->m_renderer->setFramebuffer(1);
        
        ms->m_renderer->renderWorld(m_levelState->getGame());
        
        ms->m_renderer->renderJonAndExtraForegroundObjects(m_levelState->getGame());
        
        ms->m_renderer->renderToThirdFramebufferFadeTransition(m_fTransitionStateTime);
        
        if (ms->m_renderer->isLoadingData())
        {
            ms->m_renderer->renderLoading(ms->m_fDeltaTime * ms->m_iNumInternalUpdates);
        }
        
        ms->m_renderer->renderToScreen();
        
        ms->m_renderer->endFrame();
    }
    else
    {
        m_fTransitionStateTime += ms->m_fDeltaTime * 0.5f;
        
        if (m_fTransitionStateTime < 1
            || ms->m_renderer->isLoadingData())
        {
            return;
        }
        
        if (m_fTransitionStateTime > 1)
        {
            ms->m_stateMachine.setPreviousState(Title::getInstance());
            ms->m_stateMachine.setCurrentState(m_levelState);
            ms->m_renderer->unload(RENDERER_TYPE_TITLE);
        }
    }
}

void TitleToDemo::exit(MainScreen* ms)
{
    m_levelState = nullptr;
    m_fTransitionStateTime = 0;
    m_iWorldToLoad = 0;
    m_iLevelToLoad = 0;
}

void TitleToDemo::initRenderer(MainScreen* ms)
{
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_MAP);
    ms->m_renderer->unload(RENDERER_TYPE_LEVEL_EDITOR);
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_1_CUTSCENE);
    
    ms->m_renderer->load(RENDERER_TYPE_TITLE);
    ms->m_renderer->load(calcRendererTypeFromLevel(m_iWorldToLoad, m_iLevelToLoad));
}

void TitleToDemo::setWorldToLoad(int worldToLoad)
{
    m_iWorldToLoad = worldToLoad;
}

void TitleToDemo::setLevelToLoad(int levelToLoad)
{
    m_iLevelToLoad = levelToLoad;
}

TitleToDemo::TitleToDemo() :
m_levelState(nullptr),
m_fTransitionStateTime(0),
m_iWorldToLoad(0),
m_iLevelToLoad(0)
{
    // Empty
}

/// Title To World Map Transition ///

TitleToWorldMap * TitleToWorldMap::getInstance()
{
    static TitleToWorldMap *instance = new TitleToWorldMap();
    
    return instance;
}

void TitleToWorldMap::enter(MainScreen* ms)
{
    m_fTransitionStateTime = -1;
    
    WorldMap::getInstance()->enter(ms);
}

void TitleToWorldMap::execute(MainScreen* ms)
{
    if (ms->m_isRequestingRender)
    {
        ms->m_renderer->beginFrame();
        
        ms->m_renderer->renderTitleScreenBackground(Title::getInstance()->getTitlePanel());
        
        ms->m_renderer->setFramebuffer(1);
        
        WorldMap* wm = WorldMap::getInstance();
        ms->m_renderer->renderWorldMapScreenBackground(wm->getWorldMapPanel());
        ms->m_renderer->renderWorldMapScreenUi(*wm);
        ms->m_renderer->renderWorldMapScreenButtons(*wm);
        
        ms->m_renderer->renderToThirdFramebufferTransition(m_fTransitionStateTime);
        
        if (ms->m_renderer->isLoadingData())
        {
            ms->m_renderer->renderLoading(ms->m_fDeltaTime * ms->m_iNumInternalUpdates);
        }
        
        ms->m_renderer->renderToScreen();
        
        ms->m_renderer->endFrame();
    }
    else
    {
        if (m_fTransitionStateTime < 0)
        {
            m_fTransitionStateTime = 0;
            NG_AUDIO_ENGINE->playSound(SOUND_SCREEN_TRANSITION);
            NG_AUDIO_ENGINE->stopMusic();
        }
        
        m_fTransitionStateTime += ms->m_fDeltaTime * 0.8f;
        
        if (m_fTransitionStateTime > 1)
        {
            ms->m_stateMachine.setCurrentState(WorldMap::getInstance());
            ms->m_renderer->unload(RENDERER_TYPE_TITLE);
            
            NG_AUDIO_ENGINE->loadMusic("level_select_bgm");
            NG_AUDIO_ENGINE->playMusic(true);
        }
    }
}

void TitleToWorldMap::exit(MainScreen* ms)
{
    m_fTransitionStateTime = -1;
}

void TitleToWorldMap::initRenderer(MainScreen* ms)
{
    ms->m_renderer->unload(RENDERER_TYPE_LEVEL_EDITOR);
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_1_CUTSCENE);
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_1);
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_1_MID_BOSS);
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_1_END_BOSS);
    
    ms->m_renderer->load(RENDERER_TYPE_TITLE);
    ms->m_renderer->load(RENDERER_TYPE_WORLD_MAP);
}

TitleToWorldMap::TitleToWorldMap() : m_fTransitionStateTime(0)
{
    // Empty
}

/// Title To Opening Cutscene Transition ///

TitleToOpeningCutscene * TitleToOpeningCutscene::getInstance()
{
    static TitleToOpeningCutscene *instance = new TitleToOpeningCutscene();
    
    return instance;
}

void TitleToOpeningCutscene::enter(MainScreen* ms)
{
    m_fTransitionStateTime = -1;
    
    OpeningCutscene::getInstance()->enter(ms);
}

void TitleToOpeningCutscene::execute(MainScreen* ms)
{
    if (ms->m_isRequestingRender)
    {
        ms->m_renderer->beginFrame();
        
        ms->m_renderer->renderTitleScreenBackground(Title::getInstance()->getTitlePanel());
        
        ms->m_renderer->setFramebuffer(1);
        
        ms->m_renderer->renderCutscene(OpeningCutscene::getInstance()->getCutscenePanels());
        
        ms->m_renderer->renderToThirdFramebufferTransition(m_fTransitionStateTime);
        
        if (ms->m_renderer->isLoadingData())
        {
            ms->m_renderer->renderLoading(ms->m_fDeltaTime * ms->m_iNumInternalUpdates);
        }
        
        ms->m_renderer->renderToScreen();
        
        ms->m_renderer->endFrame();
    }
    else
    {
        if (m_fTransitionStateTime < 0)
        {
            m_fTransitionStateTime = 0;
            NG_AUDIO_ENGINE->playSound(SOUND_SCREEN_TRANSITION);
            NG_AUDIO_ENGINE->stopMusic();
        }
        
        m_fTransitionStateTime += ms->m_fDeltaTime * 0.8f;
        
        if (m_fTransitionStateTime > 1)
        {
            ms->m_stateMachine.setCurrentState(OpeningCutscene::getInstance());
            ms->m_renderer->unload(RENDERER_TYPE_TITLE);
            
            NG_AUDIO_ENGINE->loadMusic("opening_cutscene_bgm");
            NG_AUDIO_ENGINE->playMusic(false);
        }
    }
}

void TitleToOpeningCutscene::exit(MainScreen* ms)
{
    m_fTransitionStateTime = -1;
}

void TitleToOpeningCutscene::initRenderer(MainScreen* ms)
{
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_MAP);
    ms->m_renderer->unload(RENDERER_TYPE_LEVEL_EDITOR);
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_1);
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_1_MID_BOSS);
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_1_END_BOSS);
    
    ms->m_renderer->load(RENDERER_TYPE_TITLE);
    ms->m_renderer->load(RENDERER_TYPE_WORLD_1_CUTSCENE);
}

TitleToOpeningCutscene::TitleToOpeningCutscene() : m_fTransitionStateTime(0)
{
    // Empty
}

/// Title To World Map Transition ///

OpeningCutsceneToWorldMap * OpeningCutsceneToWorldMap::getInstance()
{
    static OpeningCutsceneToWorldMap *instance = new OpeningCutsceneToWorldMap();
    
    return instance;
}

void OpeningCutsceneToWorldMap::enter(MainScreen* ms)
{
    m_fTransitionStateTime = -1;
    
    WorldMap::getInstance()->enter(ms);
}

void OpeningCutsceneToWorldMap::execute(MainScreen* ms)
{
    if (ms->m_isRequestingRender)
    {
        ms->m_renderer->beginFrame();
        
        ms->m_renderer->renderCutscene(OpeningCutscene::getInstance()->getCutscenePanels());
        
        ms->m_renderer->setFramebuffer(1);
        
        WorldMap* wm = WorldMap::getInstance();
        ms->m_renderer->renderWorldMapScreenBackground(wm->getWorldMapPanel());
        ms->m_renderer->renderWorldMapScreenUi(*wm);
        ms->m_renderer->renderWorldMapScreenButtons(*wm);
        
        ms->m_renderer->renderToThirdFramebufferTransition(m_fTransitionStateTime);
        
        if (ms->m_renderer->isLoadingData())
        {
            ms->m_renderer->renderLoading(ms->m_fDeltaTime * ms->m_iNumInternalUpdates);
        }
        
        ms->m_renderer->renderToScreen();
        
        ms->m_renderer->endFrame();
    }
    else
    {
        if (m_fTransitionStateTime < 0)
        {
            m_fTransitionStateTime = 0;
        }
        
        m_fTransitionStateTime += ms->m_fDeltaTime * 0.8f;
        
        if (m_fTransitionStateTime > 1)
        {
            ms->m_stateMachine.setPreviousState(Title::getInstance());
            ms->m_stateMachine.setCurrentState(WorldMap::getInstance());
            ms->m_renderer->unload(RENDERER_TYPE_WORLD_1_CUTSCENE);
            
            NG_AUDIO_ENGINE->loadMusic("level_select_bgm");
            NG_AUDIO_ENGINE->playMusic();
        }
    }
}

void OpeningCutsceneToWorldMap::exit(MainScreen* ms)
{
    m_fTransitionStateTime = -1;
}

void OpeningCutsceneToWorldMap::initRenderer(MainScreen* ms)
{
    ms->m_renderer->unload(RENDERER_TYPE_TITLE);
    ms->m_renderer->unload(RENDERER_TYPE_LEVEL_EDITOR);
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_1);
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_1_MID_BOSS);
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_1_END_BOSS);
    
    ms->m_renderer->load(RENDERER_TYPE_WORLD_MAP);
    ms->m_renderer->load(RENDERER_TYPE_WORLD_1_CUTSCENE);
}

OpeningCutsceneToWorldMap::OpeningCutsceneToWorldMap() : m_fTransitionStateTime(0)
{
    // Empty
}

/// Title To Level Editor Transition ///

TitleToLevelEditor * TitleToLevelEditor::getInstance()
{
    static TitleToLevelEditor *instance = new TitleToLevelEditor();
    
    return instance;
}

void TitleToLevelEditor::enter(MainScreen* ms)
{
    m_fTransitionStateTime = -1;
    
    MainScreenLevelEditor::getInstance()->enter(ms);
}

void TitleToLevelEditor::execute(MainScreen* ms)
{
    if (ms->m_isRequestingRender)
    {
        ms->m_renderer->beginFrame();
        
        ms->m_renderer->renderTitleScreenBackground(Title::getInstance()->getTitlePanel());
        
        ms->m_renderer->setFramebuffer(1);
        
        ms->m_renderer->renderLevelEditor(MainScreenLevelEditor::getInstance());
        
        ms->m_renderer->renderToThirdFramebufferTransition(m_fTransitionStateTime);
        
        if (ms->m_renderer->isLoadingData())
        {
            ms->m_renderer->renderLoading(ms->m_fDeltaTime * ms->m_iNumInternalUpdates);
        }
        
        ms->m_renderer->renderToScreen();
        
        ms->m_renderer->endFrame();
    }
    else
    {
        if (m_fTransitionStateTime < 0)
        {
            m_fTransitionStateTime = 0;
            NG_AUDIO_ENGINE->playSound(SOUND_SCREEN_TRANSITION_2);
            NG_AUDIO_ENGINE->stopMusic();
        }
        
        m_fTransitionStateTime += ms->m_fDeltaTime * 0.8f;
        
        if (m_fTransitionStateTime > 1)
        {
            ms->m_stateMachine.setCurrentState(MainScreenLevelEditor::getInstance());
            ms->m_renderer->unload(RENDERER_TYPE_TITLE);
        }
    }
}

void TitleToLevelEditor::exit(MainScreen* ms)
{
    m_fTransitionStateTime = -1;
}

void TitleToLevelEditor::initRenderer(MainScreen* ms)
{
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_MAP);
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_1_CUTSCENE);
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_1);
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_1_MID_BOSS);
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_1_END_BOSS);
    
    ms->m_renderer->load(RENDERER_TYPE_TITLE);
    ms->m_renderer->load(RENDERER_TYPE_LEVEL_EDITOR);
}

TitleToLevelEditor::TitleToLevelEditor() : m_fTransitionStateTime(0)
{
    // Empty
}

/// World Map to Opening Cutscene Transition ///

WorldMapToOpeningCutscene * WorldMapToOpeningCutscene::getInstance()
{
    static WorldMapToOpeningCutscene *instance = new WorldMapToOpeningCutscene();
    
    return instance;
}

void WorldMapToOpeningCutscene::enter(MainScreen* ms)
{
    m_fTransitionStateTime = 0;
    m_fFade = 0;
    m_hasPlayedTransitionSound = false;
    
    OpeningCutscene::getInstance()->enter(ms);
}

void WorldMapToOpeningCutscene::execute(MainScreen* ms)
{
    if (ms->m_isRequestingRender)
    {
        ms->m_renderer->beginFrame();
        
        ms->m_renderer->renderWorldMapScreenBackground(WorldMap::getInstance()->getWorldMapPanel());
        ms->m_renderer->renderWorldMapScreenUi(*WorldMap::getInstance());
        ms->m_renderer->renderWorldMapScreenButtons(*WorldMap::getInstance());
        
        ms->m_renderer->setFramebuffer(1);
        
        ms->m_renderer->renderCutscene(OpeningCutscene::getInstance()->getCutscenePanels());
        
        ms->m_renderer->renderToThirdFramebufferPointTransition(m_fCenterX, m_fCenterY, m_fTransitionStateTime);
        
        if (ms->m_renderer->isLoadingData())
        {
            ms->m_renderer->renderLoading(ms->m_fDeltaTime * ms->m_iNumInternalUpdates);
        }
        
        ms->m_renderer->renderToScreen();
        
        ms->m_renderer->endFrame();
    }
    else
    {
        m_fTransitionStateTime += ms->m_fDeltaTime * 0.5f;
        if (m_fTransitionStateTime > 0.5f)
        {
            if (!m_hasPlayedTransitionSound)
            {
                m_fTransitionStateTime = 0.5f;
            }
        }
        
        m_fFade = m_fTransitionStateTime * 2;
        
        WorldMap::getInstance()->updateButtons(ms->m_fDeltaTime);
        
        WorldMap::getInstance()->setFade(m_fFade);
        
        if (m_fFade < 1
            || ms->m_renderer->isLoadingData())
        {
            return;
        }
        
        if (!m_hasPlayedTransitionSound)
        {
            NG_AUDIO_ENGINE->stopMusic();
            
            m_hasPlayedTransitionSound = true;
        }
        
        if (m_fTransitionStateTime > 1)
        {
            ms->m_stateMachine.setCurrentState(OpeningCutscene::getInstance());
            ms->m_renderer->unload(RENDERER_TYPE_WORLD_MAP);
            
            NG_AUDIO_ENGINE->loadMusic("opening_cutscene_bgm");
            NG_AUDIO_ENGINE->playMusic(false);
        }
    }
}

void WorldMapToOpeningCutscene::exit(MainScreen* ms)
{
    m_fTransitionStateTime = 0;
}

void WorldMapToOpeningCutscene::initRenderer(MainScreen* ms)
{
    ms->m_renderer->unload(RENDERER_TYPE_TITLE);
    ms->m_renderer->unload(RENDERER_TYPE_LEVEL_EDITOR);
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_1);
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_1_MID_BOSS);
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_1_END_BOSS);
    
    ms->m_renderer->load(RENDERER_TYPE_WORLD_MAP);
    ms->m_renderer->load(RENDERER_TYPE_WORLD_1_CUTSCENE);
}

void WorldMapToOpeningCutscene::setCutsceneButtonLocation(float centerX, float centerY)
{
    m_fCenterX = centerX;
    m_fCenterY = centerY;
}

WorldMapToOpeningCutscene::WorldMapToOpeningCutscene() : m_fTransitionStateTime(0), m_fFade(0), m_fCenterX(0), m_fCenterY(0), m_hasPlayedTransitionSound(false)
{
    // Empty
}

/// World Map to Level Transition ///

WorldMapToLevel * WorldMapToLevel::getInstance()
{
    static WorldMapToLevel *instance = new WorldMapToLevel();
    
    return instance;
}

void WorldMapToLevel::enter(MainScreen* ms)
{
    m_fTransitionStateTime = 0;
    m_fFade = 0;
    m_hasPlayedTransitionSound = false;

    m_levelState = LevelUtil::getInstanceForWorldAndLevel(m_iWorldToLoad, m_iLevelToLoad);
    
	m_levelState->setBestStats(m_iBestScore, m_iBestOnlineScore, m_iBestLevelStatsFlag, m_iLastKnownNumGoldenCarrots, m_iLastKnownJonAbilityFlag);
    
    m_levelState->enter(ms);
}

void WorldMapToLevel::execute(MainScreen* ms)
{
    if (ms->m_isRequestingRender)
    {
        ms->m_renderer->beginFrame();
        
        ms->m_renderer->renderWorldMapScreenBackground(WorldMap::getInstance()->getWorldMapPanel());
        ms->m_renderer->renderWorldMapScreenUi(*WorldMap::getInstance());
        ms->m_renderer->renderWorldMapScreenButtons(*WorldMap::getInstance());
        
        ms->m_renderer->setFramebuffer(1);
        
        ms->m_renderer->renderWorld(m_levelState->getGame());
        
        ms->m_renderer->renderJonAndExtraForegroundObjects(m_levelState->getGame());
        
        ms->m_renderer->renderToThirdFramebufferPointTransition(m_fCenterX, m_fCenterY, m_fTransitionStateTime);
        
        if (ms->m_renderer->isLoadingData())
        {
            ms->m_renderer->renderLoading(ms->m_fDeltaTime * ms->m_iNumInternalUpdates);
        }
        
        ms->m_renderer->renderToScreen();
        
        ms->m_renderer->endFrame();
    }
    else
    {
        m_fTransitionStateTime += ms->m_fDeltaTime * 0.5f;
        if (m_fTransitionStateTime > 0.5f)
        {
            if (!m_hasPlayedTransitionSound)
            {
                m_fTransitionStateTime = 0.5f;
            }
        }
        
        m_fFade = m_fTransitionStateTime * 2;
        
        WorldMap::getInstance()->setFade(m_fFade);
        
        if (m_fFade < 1
            || ms->m_renderer->isLoadingData())
        {
            return;
        }
        
        if (!m_hasPlayedTransitionSound)
        {
            NG_AUDIO_ENGINE->playSound(SOUND_SCREEN_TRANSITION_2);
            NG_AUDIO_ENGINE->stopMusic();
            
            m_hasPlayedTransitionSound = true;
        }
        
        if (m_fTransitionStateTime > 1)
        {
            ms->m_stateMachine.setPreviousState(WorldMap::getInstance());
            ms->m_stateMachine.setCurrentState(m_levelState);
            ms->m_renderer->unload(RENDERER_TYPE_WORLD_MAP);
        }
    }
}

void WorldMapToLevel::exit(MainScreen* ms)
{
    m_levelState = nullptr;
    m_fTransitionStateTime = 0;
    m_iWorldToLoad = 0;
    m_iLevelToLoad = 0;
    
    m_iBestScore = 0;
    m_iBestOnlineScore = 0;
    m_iBestLevelStatsFlag = 0;
    m_iLastKnownNumGoldenCarrots = 0;
    m_iLastKnownJonAbilityFlag = 0;
}

void WorldMapToLevel::initRenderer(MainScreen* ms)
{
    ms->m_renderer->unload(RENDERER_TYPE_TITLE);
    ms->m_renderer->unload(RENDERER_TYPE_LEVEL_EDITOR);
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_1_CUTSCENE);
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_1);
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_1_MID_BOSS);
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_1_END_BOSS);
    
    ms->m_renderer->load(RENDERER_TYPE_WORLD_MAP);
    ms->m_renderer->load(calcRendererTypeFromLevel(m_iWorldToLoad, m_iLevelToLoad));
}

void WorldMapToLevel::setLevelLocation(float centerX, float centerY)
{
    m_fCenterX = centerX;
    m_fCenterY = centerY;
}

void WorldMapToLevel::setWorldToLoad(int worldToLoad)
{
    m_iWorldToLoad = worldToLoad;
}

void WorldMapToLevel::setLevelToLoad(int levelToLoad)
{
    m_iLevelToLoad = levelToLoad;
}

void WorldMapToLevel::setBestStats(int bestScore, int bestOnlineScore, int bestLevelStatsFlag, int numGoldenCarrots, int jonAbilityFlag)
{
    m_iBestScore = bestScore;
    m_iBestOnlineScore = bestOnlineScore;
    m_iBestLevelStatsFlag = bestLevelStatsFlag;
    m_iLastKnownNumGoldenCarrots = numGoldenCarrots;
    m_iLastKnownJonAbilityFlag = jonAbilityFlag;
}

WorldMapToLevel::WorldMapToLevel() :
m_levelState(nullptr),
m_fCenterX(0),
m_fCenterY(0),
m_fTransitionStateTime(0),
m_fFade(0),
m_iWorldToLoad(0),
m_iLevelToLoad(0),
m_iBestScore(0),
m_iBestOnlineScore(0),
m_iBestLevelStatsFlag(0),
m_iLastKnownNumGoldenCarrots(0),
m_iLastKnownJonAbilityFlag(0),
m_hasPlayedTransitionSound(false)
{
    // Empty
}

/// Level To Coming Soon Transition ///

LevelToComingSoon * LevelToComingSoon::getInstance()
{
    static LevelToComingSoon *instance = new LevelToComingSoon();
    
    return instance;
}

void LevelToComingSoon::enter(MainScreen* ms)
{
    m_fTransitionStateTime = -1;
    
    ComingSoon::getInstance()->enter(ms);
}

void LevelToComingSoon::execute(MainScreen* ms)
{
    if (ms->m_isRequestingRender)
    {
        ms->m_renderer->beginFrame();
        
        if (m_levelState)
        {
            ms->m_renderer->renderHud(m_levelState->getGame(), nullptr, m_levelState->getContinueButton(), m_levelState->getScore());
        }
        
        ms->m_renderer->setFramebuffer(1);
        
        ms->m_renderer->renderComingSoonScreenBackground();
        
        ms->m_renderer->renderToThirdFramebufferFadeTransition(m_fTransitionStateTime);
        
        if (ms->m_renderer->isLoadingData())
        {
            ms->m_renderer->renderLoading(ms->m_fDeltaTime * ms->m_iNumInternalUpdates);
        }
        
        ms->m_renderer->renderToScreen();
        
        ms->m_renderer->endFrame();
    }
    else
    {
        if (m_fTransitionStateTime < 0)
        {
            m_fTransitionStateTime = 0;
            
            NG_AUDIO_ENGINE->stopMusic();
        }
        
        m_fTransitionStateTime += ms->m_fDeltaTime * 0.8f;
        
        if (m_fTransitionStateTime > 1)
        {
            ms->m_stateMachine.setCurrentState(ComingSoon::getInstance());
            
            NG_AUDIO_ENGINE->loadMusic("title_bgm");
            NG_AUDIO_ENGINE->playMusic(true);
        }
    }
}

void LevelToComingSoon::exit(MainScreen* ms)
{
    m_fTransitionStateTime = -1;
}

void LevelToComingSoon::initRenderer(MainScreen* ms)
{
    ms->m_renderer->unload(RENDERER_TYPE_TITLE);
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_MAP);
    ms->m_renderer->unload(RENDERER_TYPE_LEVEL_EDITOR);
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_1_CUTSCENE);
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_1_MID_BOSS);
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_1_END_BOSS);
    
    ms->m_renderer->load(RENDERER_TYPE_WORLD_1);
}

void LevelToComingSoon::setLevelComingFrom(Level* levelState)
{
    m_levelState = levelState;
}

LevelToComingSoon::LevelToComingSoon() : m_levelState(nullptr), m_fTransitionStateTime(0)
{
    // Empty
}

RTTI_IMPL(TitleToDemo, MainScreenState);
RTTI_IMPL(TitleToWorldMap, MainScreenState);
RTTI_IMPL(TitleToOpeningCutscene, MainScreenState);
RTTI_IMPL(OpeningCutsceneToWorldMap, MainScreenState);
RTTI_IMPL(TitleToLevelEditor, MainScreenState);
RTTI_IMPL(WorldMapToOpeningCutscene, MainScreenState);
RTTI_IMPL(WorldMapToLevel, MainScreenState);
RTTI_IMPL(LevelToComingSoon, MainScreenState);
