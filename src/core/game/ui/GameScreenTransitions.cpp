//
//  GameScreenTransitions.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/10/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "GameScreenTransitions.h"

#include "State.h"
#include "GameScreen.h"
#include "EntityUtils.h"
#include "Vector2D.h"
#include "Game.h"
#include "GameScreenLevelEditor.h"
#include "GameButton.h"
#include "GameScreenTitle.h"
#include "GameScreenWorldMap.h"
#include "GameScreenOpeningCutscene.h"
#include "BatPanel.h"

/// Title To World Map Transition ///

TitleToWorldMap * TitleToWorldMap::getInstance()
{
    static TitleToWorldMap *instance = new TitleToWorldMap();
    
    return instance;
}

void TitleToWorldMap::enter(GameScreen* gs)
{
    m_fTransitionStateTime = -1;
    WorldMap::getInstance()->enter(gs);
}

void TitleToWorldMap::execute(GameScreen* gs)
{
    if (gs->m_isRequestingRender)
    {
        gs->m_renderer->beginFrame(gs->m_fDeltaTime);
        
        gs->m_renderer->renderTitleScreenBackground(Title::getInstance()->getTitlePanel());
        
        gs->m_renderer->setFramebuffer(1);
        
        WorldMap* wm = WorldMap::getInstance();
        gs->m_renderer->renderWorldMapScreenBackground(wm->getWorldMapPanel());
        gs->m_renderer->renderWorldMapScreenUi(*wm);
        gs->m_renderer->renderWorldMapScreenButtons(*wm);
        
        gs->m_renderer->renderToScreenTransition(m_fTransitionStateTime);
        
        if (gs->m_renderer->isLoadingAdditionalTextures())
        {
            gs->m_renderer->renderLoading();
        }
        
        gs->m_renderer->endFrame();
    }
    else
    {
        if (m_fTransitionStateTime < 0)
        {
            m_fTransitionStateTime = 0;
            Assets::getInstance()->addSoundIdToPlayQueue(SOUND_SCREEN_TRANSITION);
            Assets::getInstance()->setMusicId(MUSIC_STOP);
        }
        
        m_fTransitionStateTime += gs->m_fDeltaTime * 0.8f;
        
        if (m_fTransitionStateTime > 1)
        {
            gs->m_stateMachine->setCurrentState(WorldMap::getInstance());
            gs->m_renderer->unload(RENDERER_TYPE_TITLE);
            
            Assets::getInstance()->setMusicId(MUSIC_PLAY_LEVEL_SELECT_LOOP);
        }
    }
}

void TitleToWorldMap::exit(GameScreen* gs)
{
    m_fTransitionStateTime = -1;
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

void TitleToOpeningCutscene::enter(GameScreen* gs)
{
    m_fTransitionStateTime = -1;
    OpeningCutscene::getInstance()->enter(gs);
}

void TitleToOpeningCutscene::execute(GameScreen* gs)
{
    if (gs->m_isRequestingRender)
    {
        gs->m_renderer->beginFrame(gs->m_fDeltaTime);
        
        gs->m_renderer->renderTitleScreenBackground(Title::getInstance()->getTitlePanel());
        
        gs->m_renderer->setFramebuffer(1);
        
        gs->m_renderer->renderCutscene(OpeningCutscene::getInstance()->getCutscenePanels());
        
        gs->m_renderer->renderToScreenTransition(m_fTransitionStateTime);
        
        if (gs->m_renderer->isLoadingAdditionalTextures())
        {
            gs->m_renderer->renderLoading();
        }
        
        gs->m_renderer->endFrame();
    }
    else
    {
        if (m_fTransitionStateTime < 0)
        {
            m_fTransitionStateTime = 0;
            Assets::getInstance()->addSoundIdToPlayQueue(SOUND_SCREEN_TRANSITION);
            Assets::getInstance()->setMusicId(MUSIC_STOP);
        }
        
        m_fTransitionStateTime += gs->m_fDeltaTime * 0.8f;
        
        if (m_fTransitionStateTime > 1)
        {
            gs->m_stateMachine->setCurrentState(OpeningCutscene::getInstance());
            gs->m_renderer->unload(RENDERER_TYPE_TITLE);
            
            Assets::getInstance()->setMusicId(MUSIC_PLAY_OPENING_CUTSCENE);
        }
    }
}

void TitleToOpeningCutscene::exit(GameScreen* gs)
{
    m_fTransitionStateTime = -1;
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

void OpeningCutsceneToWorldMap::enter(GameScreen* gs)
{
    m_fTransitionStateTime = -1;
    WorldMap::getInstance()->enter(gs);
}

void OpeningCutsceneToWorldMap::execute(GameScreen* gs)
{
    if (gs->m_isRequestingRender)
    {
        gs->m_renderer->beginFrame(gs->m_fDeltaTime);
        
        gs->m_renderer->renderCutscene(OpeningCutscene::getInstance()->getCutscenePanels());
        
        gs->m_renderer->setFramebuffer(1);
        
        WorldMap* wm = WorldMap::getInstance();
        gs->m_renderer->renderWorldMapScreenBackground(wm->getWorldMapPanel());
        gs->m_renderer->renderWorldMapScreenUi(*wm);
        gs->m_renderer->renderWorldMapScreenButtons(*wm);
        
        gs->m_renderer->renderToScreenTransition(m_fTransitionStateTime);
        
        if (gs->m_renderer->isLoadingAdditionalTextures())
        {
            gs->m_renderer->renderLoading();
        }
        
        gs->m_renderer->endFrame();
    }
    else
    {
        if (m_fTransitionStateTime < 0)
        {
            m_fTransitionStateTime = 0;
        }
        
        m_fTransitionStateTime += gs->m_fDeltaTime * 0.8f;
        
        if (m_fTransitionStateTime > 1)
        {
            gs->m_stateMachine->setPreviousState(Title::getInstance());
            gs->m_stateMachine->setCurrentState(WorldMap::getInstance());
            gs->m_renderer->unload(RENDERER_TYPE_WORLD_1_CUTSCENE);
            
            Assets::getInstance()->setMusicId(MUSIC_PLAY_LEVEL_SELECT_LOOP);
        }
    }
}

void OpeningCutsceneToWorldMap::exit(GameScreen* gs)
{
    m_fTransitionStateTime = -1;
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

void TitleToLevelEditor::enter(GameScreen* gs)
{
    m_fTransitionStateTime = -1;
    GameScreenLevelEditor::getInstance()->enter(gs);
}

void TitleToLevelEditor::execute(GameScreen* gs)
{
    if (gs->m_isRequestingRender)
    {
        gs->m_renderer->beginFrame(gs->m_fDeltaTime);
        
        gs->m_renderer->renderTitleScreenBackground(Title::getInstance()->getTitlePanel());
        
        gs->m_renderer->setFramebuffer(1);
        
        gs->m_renderer->renderLevelEditor(GameScreenLevelEditor::getInstance());
        
        gs->m_renderer->renderToScreenTransition(m_fTransitionStateTime);
        
        if (gs->m_renderer->isLoadingAdditionalTextures())
        {
            gs->m_renderer->renderLoading();
        }
        
        gs->m_renderer->endFrame();
    }
    else
    {
        if (m_fTransitionStateTime < 0)
        {
            m_fTransitionStateTime = 0;
            Assets::getInstance()->addSoundIdToPlayQueue(SOUND_SCREEN_TRANSITION_2);
            Assets::getInstance()->setMusicId(MUSIC_STOP);
        }
        
        m_fTransitionStateTime += gs->m_fDeltaTime * 0.8f;
        
        if (m_fTransitionStateTime > 1)
        {
            gs->m_stateMachine->setCurrentState(GameScreenLevelEditor::getInstance());
            gs->m_renderer->unload(RENDERER_TYPE_TITLE);
        }
    }
}

void TitleToLevelEditor::exit(GameScreen* gs)
{
    m_fTransitionStateTime = -1;
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

void WorldMapToOpeningCutscene::enter(GameScreen* gs)
{
    m_fTransitionStateTime = 0;
    m_fFade = 0;
    m_hasPlayedTransitionSound = false;
    OpeningCutscene::getInstance()->enter(gs);
}

void WorldMapToOpeningCutscene::execute(GameScreen* gs)
{
    if (gs->m_isRequestingRender)
    {
        gs->m_renderer->beginFrame(gs->m_fDeltaTime);
        
        m_fTransitionStateTime += gs->m_fDeltaTime * 0.5f;
        if (m_fTransitionStateTime > 0.5f)
        {
            if (!m_hasPlayedTransitionSound)
            {
                m_fTransitionStateTime = 0.5f;
            }
        }
        
        m_fFade = m_fTransitionStateTime * 2;
        
        WorldMap::getInstance()->setFade(m_fFade);
        WorldMap::getInstance()->updateButtons(gs->m_fDeltaTime);
        
        gs->m_renderer->renderWorldMapScreenBackground(WorldMap::getInstance()->getWorldMapPanel());
        gs->m_renderer->renderWorldMapScreenUi(*WorldMap::getInstance());
        gs->m_renderer->renderWorldMapScreenButtons(*WorldMap::getInstance());
        
        gs->m_renderer->setFramebuffer(1);
        
        gs->m_renderer->renderCutscene(OpeningCutscene::getInstance()->getCutscenePanels());
        
        gs->m_renderer->renderToScreenPointTransition(m_fCenterX, m_fCenterY, m_fTransitionStateTime);
        
        if (gs->m_renderer->isLoadingAdditionalTextures())
        {
            gs->m_renderer->renderLoading();
        }
        
        gs->m_renderer->endFrame();
    }
    else
    {
        if (m_fFade < 1)
        {
            return;
        }
        
        if (!m_hasPlayedTransitionSound)
        {
            Assets::getInstance()->setMusicId(MUSIC_STOP);
            
            m_hasPlayedTransitionSound = true;
        }
        
        if (m_fTransitionStateTime > 1)
        {
            gs->m_stateMachine->setCurrentState(OpeningCutscene::getInstance());
            gs->m_renderer->unload(RENDERER_TYPE_WORLD_MAP);
            
            Assets::getInstance()->setMusicId(MUSIC_PLAY_OPENING_CUTSCENE);
        }
    }
}

void WorldMapToOpeningCutscene::exit(GameScreen* gs)
{
    m_fTransitionStateTime = 0;
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

void WorldMapToLevel::enter(GameScreen* gs)
{
    m_levelState = LevelUtil::getInstanceForWorldAndLevel(m_iWorldToLoad, m_iLevelToLoad);
    
	m_levelState->setBestStats(m_iBestScore, m_iBestOnlineScore, m_iBestLevelStatsFlag, m_iLastKnownNumGoldenCarrots, m_iLastKnownJonAbilityFlag);
    
    m_levelState->enter(gs);
    
    m_fTransitionStateTime = 0;
    m_fFade = 0;
    m_hasPlayedTransitionSound = false;
}

void WorldMapToLevel::execute(GameScreen* gs)
{
    if (gs->m_isRequestingRender)
    {
        gs->m_renderer->beginFrame(gs->m_fDeltaTime);
        
        m_fTransitionStateTime += gs->m_fDeltaTime * 0.5f;
        if (m_fTransitionStateTime > 0.5f)
        {
            if (!m_hasPlayedTransitionSound)
            {
                m_fTransitionStateTime = 0.5f;
            }
        }
        
        m_fFade = m_fTransitionStateTime * 2;
        
        WorldMap::getInstance()->setFade(m_fFade);
        
        gs->m_renderer->renderWorldMapScreenBackground(WorldMap::getInstance()->getWorldMapPanel());
        gs->m_renderer->renderWorldMapScreenUi(*WorldMap::getInstance());
        gs->m_renderer->renderWorldMapScreenButtons(*WorldMap::getInstance());
        
        gs->m_renderer->setFramebuffer(1);
        
        gs->m_renderer->renderWorld(m_levelState->getGame());
        
        gs->m_renderer->renderJonAndExtraForegroundObjects(m_levelState->getGame());
        
        gs->m_renderer->renderToScreenPointTransition(m_fCenterX, m_fCenterY, m_fTransitionStateTime);
        
        if (gs->m_renderer->isLoadingAdditionalTextures())
        {
            gs->m_renderer->renderLoading();
        }
        
        gs->m_renderer->endFrame();
    }
    else
    {
        if (m_fFade < 1)
        {
            return;
        }
        
        if (!m_hasPlayedTransitionSound)
        {
            Assets::getInstance()->addSoundIdToPlayQueue(SOUND_SCREEN_TRANSITION_2);
            Assets::getInstance()->setMusicId(MUSIC_STOP);
            
            m_hasPlayedTransitionSound = true;
        }
        
        if (m_fTransitionStateTime > 1)
        {
            gs->m_stateMachine->setPreviousState(WorldMap::getInstance());
            gs->m_stateMachine->setCurrentState(m_levelState);
            gs->m_renderer->unload(RENDERER_TYPE_WORLD_MAP);
        }
    }
}

void WorldMapToLevel::exit(GameScreen* gs)
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
