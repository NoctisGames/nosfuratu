//
//  GameScreenTransitions.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/10/16.
//  Copyright © 2016 Gowen Game Dev. All rights reserved.
//

#include "GameScreenTransitions.h"
#include "State.h"
#include "GameScreen.h"
#include "EntityUtils.h"
#include "Vector2D.h"
#include "Game.h"
#include "GameScreenLevelEditor.h"
#include "BackButton.h"
#include "LevelEditorButton.h"
#include "GameScreenTitle.h"
#include "GameScreenWorldMap.h"

/// Title To World Map Transition ///

TitleToWorldMap * TitleToWorldMap::getInstance()
{
    static TitleToWorldMap *instance = new TitleToWorldMap();
    
    return instance;
}

void TitleToWorldMap::enter(GameScreen* gs)
{
    m_fTransitionStateTime = 0;
    WorldMap::getInstance()->enter(gs);
}

void TitleToWorldMap::execute(GameScreen* gs)
{
    if (gs->m_isRequestingRender)
    {
        gs->m_renderer->beginFrame();
        
        gs->m_renderer->renderTitleScreen();
        gs->m_renderer->renderTitleScreenUi(Title::getInstance()->getLevelEditorButton());
        gs->m_renderer->renderLoadingTextOnTitleScreen();
        
        gs->m_renderer->setFramebuffer(1);
        
        gs->m_renderer->renderWorldMapScreenBackground();
        gs->m_renderer->renderWorldMapScreenUi(WorldMap::getInstance()->getBackButton());
        
        gs->m_renderer->renderToScreenTransition(m_fTransitionStateTime);
        
        gs->m_renderer->endFrame();
    }
    else
    {
        m_fTransitionStateTime += gs->m_fDeltaTime;
        
        if (m_fTransitionStateTime > 1)
        {
            gs->m_stateMachine->setCurrentState(WorldMap::getInstance());
        }
    }
}

void TitleToWorldMap::exit(GameScreen* gs)
{
    m_fTransitionStateTime = 0;
}

TitleToWorldMap::TitleToWorldMap() : m_fTransitionStateTime(0)
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
    m_fTransitionStateTime = 0;
    GameScreenLevelEditor::getInstance()->enter(gs);
}

void TitleToLevelEditor::execute(GameScreen* gs)
{
    if (gs->m_isRequestingRender)
    {
        gs->m_renderer->beginFrame();
        
        gs->m_renderer->renderTitleScreen();
        gs->m_renderer->renderTitleScreenUi(Title::getInstance()->getLevelEditorButton());
        gs->m_renderer->renderLoadingTextOnTitleScreen();
        
        gs->m_renderer->setFramebuffer(1);
        
        gs->m_renderer->renderWorld(GameScreenLevelEditor::getInstance()->getGame());
        
        gs->m_renderer->renderJon(GameScreenLevelEditor::getInstance()->getGame());
        
        gs->m_renderer->renderLevelEditor(GameScreenLevelEditor::getInstance()->getLevelEditorActionsPanel(), GameScreenLevelEditor::getInstance()->getLevelEditorEntitiesPanel(), GameScreenLevelEditor::getInstance()->getTrashCan(), GameScreenLevelEditor::getInstance()->getLevelSelectorPanel());
        
        gs->m_renderer->renderToScreenTransition(m_fTransitionStateTime);
        
        gs->m_renderer->endFrame();
    }
    else
    {
        m_fTransitionStateTime += gs->m_fDeltaTime;
        
        if (m_fTransitionStateTime > 1)
        {
            gs->m_stateMachine->setCurrentState(GameScreenLevelEditor::getInstance());
        }
    }
}

void TitleToLevelEditor::exit(GameScreen* gs)
{
    m_fTransitionStateTime = 0;
}

TitleToLevelEditor::TitleToLevelEditor() : m_fTransitionStateTime(0)
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
    m_fTransitionStateTime = 0;
    
    switch (m_iLevelToLoad)
    {
        case 1:
            m_levelState = Chapter1Level1::getInstance();
            break;
        case 2:
            m_levelState = Chapter1Level2::getInstance();
            break;
        case 3:
            m_levelState = Chapter1Level3::getInstance();
            break;
        default:
            break;
    }
    
    m_levelState->enter(gs);
}

void WorldMapToLevel::execute(GameScreen* gs)
{
    if (gs->m_isRequestingRender)
    {
        gs->m_renderer->beginFrame();
        
        gs->m_renderer->renderWorldMapScreenBackground();
        gs->m_renderer->renderLoadingTextOnWorldMapScreen();
        
        gs->m_renderer->setFramebuffer(1);
        
        gs->m_renderer->renderWorld(m_levelState->getGame());
        
        gs->m_renderer->renderJon(m_levelState->getGame());
        
        gs->m_renderer->renderToScreenTransition(m_fTransitionStateTime);
        
        gs->m_renderer->endFrame();
    }
    else
    {
        gs->processTouchEvents();
        
        m_fTransitionStateTime += gs->m_fDeltaTime;
        
        if (m_fTransitionStateTime > 1)
        {
            gs->m_stateMachine->setPreviousState(WorldMap::getInstance());
            gs->m_stateMachine->setCurrentState(m_levelState);
        }
        
        m_levelState->execute(gs);
    }
}

void WorldMapToLevel::exit(GameScreen* gs)
{
    m_levelState = nullptr;
    m_fTransitionStateTime = 0;
    m_iLevelToLoad = 0;
}

void WorldMapToLevel::setLevelToLoad(int levelToLoad)
{
    m_iLevelToLoad = levelToLoad;
}

WorldMapToLevel::WorldMapToLevel() : m_levelState(nullptr), m_fTransitionStateTime(0), m_iLevelToLoad(0)
{
    // Empty
}
