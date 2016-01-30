//
//  GameScreenStates.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "GameScreenStates.h"
#include "State.h"
#include "GameScreen.h"
#include "EntityUtils.h"
#include "Vector2D.h"
#include "Game.h"
#include "Chapter1Levels.h"

/// Title Screen ///

Title * Title::getInstance()
{
    static Title *instance = new Title();
    
    return instance;
}

void Title::enter(GameScreen* gs)
{
    gs->m_stateMachine->setPreviousState(nullptr);
    gs->m_renderer->init(RENDERER_TYPE_TITLE);
}

void Title::execute(GameScreen* gs)
{
    if (gs->m_isRequestingRender)
    {
        gs->m_renderer->beginFrame();
        
        gs->m_renderer->renderTitleScreen();
        
        if (m_isRequestingNextState)
        {
            gs->m_renderer->renderLoadingTextOnTitleScreen();
        }
        
        gs->m_renderer->renderToScreen();
        
        gs->m_renderer->endFrame();
    }
    else
    {
        if (m_isRequestingNextState)
        {
            gs->m_stateMachine->changeState(TitleToWorldMap::getInstance());
        }
        
        gs->processTouchEvents();
        
        for (std::vector<TouchEvent>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
        {
            switch (i->getTouchType())
            {
                case DOWN:
                    continue;
                case DRAGGED:
                    continue;
                case UP:
                    m_isRequestingNextState = true;
            }
        }
    }
}

void Title::exit(GameScreen* gs)
{
    m_isRequestingNextState = false;
}

Title::Title() : m_isRequestingNextState(false)
{
    // Empty
}

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
        gs->m_renderer->renderLoadingTextOnTitleScreen();
        
        gs->m_renderer->setFramebuffer(1);
        
        gs->m_renderer->renderWorldMapScreenBackground();
        gs->m_renderer->renderWorldMapScreenUi(WorldMap::getInstance()->getBackButton());
        
        gs->m_renderer->renderToScreenTitleToWorldMapTransition(m_fTransitionStateTime * 2);
        
        gs->m_renderer->endFrame();
    }
    else
    {
        m_fTransitionStateTime += gs->m_fDeltaTime;
        
        if (m_fTransitionStateTime > 0.5f)
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

/// World Map Screen ///

WorldMap * WorldMap::getInstance()
{
    static WorldMap *instance = new WorldMap();
    
    return instance;
}

void WorldMap::enter(GameScreen* gs)
{
	gs->m_stateMachine->setPreviousState(Title::getInstance());
    gs->m_renderer->init(RENDERER_TYPE_TITLE);
    gs->m_iNumFramesToDiscard = 1;
}

void WorldMap::execute(GameScreen* gs)
{
    if (gs->m_isRequestingRender)
    {
        gs->m_renderer->beginFrame();
        
        gs->m_renderer->renderWorldMapScreenBackground();
        
        if (m_iLevelToLoad > 0)
        {
            gs->m_renderer->renderLoadingTextOnWorldMapScreen();
        }
        else
        {
            gs->m_renderer->renderWorldMapScreenUi(*m_backButton);
        }
        
        gs->m_renderer->renderToScreen();
        
        gs->m_renderer->endFrame();
    }
    else
    {
        if (m_iLevelToLoad > 0)
        {
            WorldMapToLevel::getInstance()->setLevelToLoad(m_iLevelToLoad);
            gs->m_stateMachine->changeState(WorldMapToLevel::getInstance());
        }
        
        gs->processTouchEvents();

        for (std::vector<TouchEvent>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
        {
            gs->touchToWorld((*i));
            
            switch (i->getTouchType())
            {
                case DOWN:
                    continue;
                case DRAGGED:
                    continue;
                case UP:
                    if (OverlapTester::isPointInRectangle(*gs->m_touchPoint, m_backButton->getBounds()))
                    {
                        gs->m_stateMachine->revertToPreviousState();
						return;
                    }
                    else if (gs->m_touchPoint->getY() < (CAM_HEIGHT * 2 / 3))
                    {
                        if (gs->m_touchPoint->getX() > CAM_WIDTH * 2 / 3)
                        {
                            m_iLevelToLoad = 3;
                        }
                        else if (gs->m_touchPoint->getX() > CAM_WIDTH / 3)
                        {
                            m_iLevelToLoad = 2;
                        }
                        else if (gs->m_touchPoint->getX() > 0)
                        {
                            m_iLevelToLoad = 1;
                        }
                    }
            }
        }
    }
}

void WorldMap::exit(GameScreen* gs)
{
    m_iLevelToLoad = 0;
}

BackButton& WorldMap::getBackButton()
{
    return *m_backButton;
}

WorldMap::WorldMap() : m_iLevelToLoad(0)
{
    m_backButton = std::unique_ptr<BackButton>(new BackButton());
}

/// Game Play ///

GamePlay * GamePlay::getInstance()
{
    static GamePlay *instance = new GamePlay(nullptr);
    
    return instance;
}

void GamePlay::enter(GameScreen* gs)
{
    m_fStateTime = 0;
    m_isReleasingShockwave = false;
    gs->m_isScreenHeldDown = false;
    gs->m_fScreenHeldTime = 0;
    
    if (!m_game->isLoaded())
    {
        if (m_sourceGame)
        {
            m_game->copy(m_sourceGame);
        }
        else
        {
            m_game->load(m_json);
        }
    }
    
    gs->m_renderer->init(RENDERER_TYPE_WORLD_1);
    
    if (m_hasShownOpeningSequence)
    {
        Jon& jon = m_game->getJon();
        jon.setAllowedToMove(true);
    }
    else
    {
        Assets::getInstance()->setMusicId(MUSIC_PLAY_DEMO);
        
        gs->processTouchEvents();
        gs->m_renderer->beginOpeningPanningSequence(*m_game);
        gs->m_iNumFramesToDiscard = 1;
        
        m_hasShownOpeningSequence = true;
    }
}

void GamePlay::execute(GameScreen* gs)
{
    Jon& jon = m_game->getJon();
    
    if (gs->m_isRequestingRender)
    {
        gs->m_renderer->beginFrame();
        
        gs->m_renderer->renderWorld(*m_game);
        
        if (m_isReleasingShockwave)
        {
            Jon& jon = m_game->getJon();
            gs->m_renderer->renderToSecondFramebufferWithShockwave(m_fShockwaveCenterX, m_fShockwaveCenterY, m_fShockwaveElapsedTime, jon.isTransformingIntoVampire() || jon.isVampire());
        }
        else
        {
            gs->m_renderer->renderToSecondFramebuffer(*m_game);
        }
        
        gs->m_renderer->renderJon(*m_game);
        
        gs->m_renderer->renderHud(*m_game, *m_backButton);
        
        if (jon.isDead())
        {
            gs->m_renderer->renderToScreenWithTransDeathIn(m_fStateTime);
        }
        else if (m_showDeathTransOut)
        {
            gs->m_renderer->renderToScreenWithTransDeathOut(m_fStateTime);
        }
        else
        {
            if (!m_hasOpeningSequenceCompleted)
            {
                gs->m_renderer->renderToScreenWithRadialBlur();
            }
            else
            {
                gs->m_renderer->renderToScreen();
            }
        }
        
        gs->m_renderer->endFrame();
    }
    else
    {
        if (!m_hasOpeningSequenceCompleted)
        {
            if (handleOpeningSequenceTouchInput(gs))
            {
                gs->m_renderer->zoomIn();
                m_hasOpeningSequenceCompleted = true;
                jon.setAllowedToMove(m_hasOpeningSequenceCompleted);
                return;
            }
            
            jon.update(gs->m_fDeltaTime);
            
            m_hasOpeningSequenceCompleted = gs->m_renderer->updateCameraToFollowPathToJon(*m_game, gs->m_fDeltaTime);
            jon.setAllowedToMove(m_hasOpeningSequenceCompleted);
            
            EntityUtils::updateBackgrounds(m_game->getBackgroundSkies(), gs->m_renderer->getCameraPosition());
            EntityUtils::updateBackgrounds(m_game->getBackgroundTrees(), gs->m_renderer->getCameraPosition());
            EntityUtils::updateBackgrounds(m_game->getBackgroundCaves(), gs->m_renderer->getCameraPosition());
            
            return;
        }
        
        if (handleTouchInput(gs))
        {
            return;
        }
        
        if (m_showDeathTransOut)
        {
            // Starting new game after death
            
            m_fStateTime += gs->m_fDeltaTime;
            
            if (m_fStateTime > 2.4f)
            {
                m_fStateTime = 0;
                m_showDeathTransOut = false;
            }
        }
        
        if (jon.isDead())
        {
            // Starting death transition, when screen goes black, new game begins
            
            m_fStateTime += gs->m_fDeltaTime;
            
            if (m_fStateTime > 1.6f)
            {
                m_game->reset();
                enter(gs);
                m_showDeathTransOut = true;
            }
            
            return;
        }
        else if (jon.getPosition().getX() - jon.getWidth() > m_game->getFarRight())
        {
            // Has Cleared the Level
            
            Assets::getInstance()->addSoundIdToPlayQueue(SOUND_STOP_JON_VAMPIRE_GLIDE);
            
            m_fStateTime += gs->m_fDeltaTime;
            
            if (m_fStateTime > 5)
            {
                m_game->reset();
                enter(gs);
                
                return;
            }
        }
        else
        {
            // Is Still Actively playing the Level
            
            m_game->update(gs->m_fDeltaTime);
            
            if (jon.isTransformingIntoVampire() || jon.isRevertingToRabbit())
            {
                if (jon.getTransformStateTime() < 0.125f)
                {
                    gs->m_fDeltaTime /= 8;
                }
                else
                {
                    if (!m_isReleasingShockwave)
                    {
                        m_fShockwaveCenterX = jon.getPosition().getX();
                        m_fShockwaveCenterY = jon.getPosition().getY();
                        m_fShockwaveElapsedTime = 0.0f;
                        m_isReleasingShockwave = true;
                    }
                }
            }
            
            if (m_isReleasingShockwave)
            {
                m_fShockwaveElapsedTime += gs->m_fDeltaTime;
                
                if (m_fShockwaveElapsedTime > 4)
                {
                    m_fShockwaveElapsedTime = 0;
                    m_isReleasingShockwave = false;
                }
            }
            
            m_game->updateAndClean(gs->m_fDeltaTime);
            
            EntityUtils::handleCollections(jon, m_game->getCarrots(), gs->m_fDeltaTime);
            EntityUtils::handleCollections(jon, m_game->getGoldenCarrots(), gs->m_fDeltaTime);
            
            if (gs->m_isScreenHeldDown)
            {
                gs->m_fScreenHeldTime += gs->m_fDeltaTime;
                
                if (gs->m_fScreenHeldTime > 0.4f)
                {
                    jon.triggerTransform();
                    gs->m_isScreenHeldDown = false;
                    m_fShockwaveElapsedTime = 0;
                    m_isReleasingShockwave = false;
                }
            }
        }
        
        gs->m_renderer->updateCameraToFollowJon(*m_game, gs->m_fDeltaTime);
        
        EntityUtils::updateBackgrounds(m_game->getBackgroundSkies(), gs->m_renderer->getCameraPosition());
        EntityUtils::updateBackgrounds(m_game->getBackgroundTrees(), gs->m_renderer->getCameraPosition());
        EntityUtils::updateBackgrounds(m_game->getBackgroundCaves(), gs->m_renderer->getCameraPosition());
    }
}

void GamePlay::exit(GameScreen* gs)
{
    m_game->reset();
    
    m_fStateTime = 0;
    m_isReleasingShockwave = false;
    m_fShockwaveElapsedTime = 0;
    m_fShockwaveCenterX = 0;
    m_fShockwaveCenterY = 0;
    m_hasShownOpeningSequence = false;
    m_hasOpeningSequenceCompleted = false;
    m_hasSwiped = false;
    m_showDeathTransOut = false;
}

void GamePlay::setSourceGame(Game* game)
{
    m_sourceGame = game;
}

Game& GamePlay::getGame()
{
    return *m_game;
}

BackButton& GamePlay::getBackButton()
{
    return *m_backButton;
}

bool GamePlay::handleOpeningSequenceTouchInput(GameScreen* gs)
{
    gs->processTouchEvents();
    
    for (std::vector<TouchEvent>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
    {
        switch (i->getTouchType())
        {
            case DOWN:
                return true;
            case DRAGGED:
                continue;
            case UP:
                continue;
        }
    }
    
    return false;
}

bool GamePlay::handleTouchInput(GameScreen* gs)
{
    gs->processTouchEvents();
    
    Jon& jon = m_game->getJon();
    bool isJonAlive = jon.isAlive();
    
    for (std::vector<TouchEvent>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
    {
        gs->touchToWorld((*i));
        
        switch (i->getTouchType())
        {
            case DOWN:
                if (isJonAlive)
                {
                    gs->m_touchPointDown->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());
                    gs->m_isScreenHeldDown = true;
                    gs->m_fScreenHeldTime = 0.0f;
                }
                continue;
            case DRAGGED:
                if (isJonAlive && !m_hasSwiped)
                {
                    if (gs->m_touchPoint->getX() >= (gs->m_touchPointDown->getX() + SWIPE_WIDTH))
                    {
                        // Swipe Right
                        jon.triggerRightAction();
                        m_hasSwiped = true;
                    }
                    else if (gs->m_touchPoint->getX() <= (gs->m_touchPointDown->getX() - SWIPE_WIDTH))
                    {
                        // Swipe Left
                        jon.triggerLeftAction();
                        m_hasSwiped = true;
                    }
                    else if (gs->m_touchPoint->getY() >= (gs->m_touchPointDown->getY() + SWIPE_HEIGHT))
                    {
                        // Swipe Up
                        jon.triggerUpAction();
                        m_hasSwiped = true;
                    }
                    else if (gs->m_touchPoint->getY() <= (gs->m_touchPointDown->getY() - SWIPE_HEIGHT))
                    {
                        // Swipe Down
                        jon.triggerDownAction();
                        m_hasSwiped = true;
                    }
                    
                    if (m_hasSwiped)
                    {
                        gs->m_isScreenHeldDown = false;
                        gs->m_fScreenHeldTime = 0.0f;
                    }
                }
                continue;
            case UP:
                if (OverlapTester::isPointInRectangle(*gs->m_touchPoint, m_backButton->getBounds()))
                {
                    Assets::getInstance()->setMusicId(MUSIC_STOP);
                    
                    gs->m_stateMachine->revertToPreviousState();
                    
                    return true;
                }
                
                if (isJonAlive)
                {
                    if (!m_hasSwiped && gs->m_fScreenHeldTime < 0.4f)
                    {
                        jon.triggerJump();
                    }
                    
                    
                    if (gs->m_fScreenHeldTime > 0.4f)
                    {
                        jon.triggerCancelTransform();
                    }
                    
                    gs->m_isScreenHeldDown = false;
                    gs->m_fScreenHeldTime = 0;
                    
                    m_hasSwiped = false;
                    
                    gs->m_touchPointDown->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());
                }
                
                break;
        }
    }
    
    return false;
}

GamePlay::GamePlay(const char* json) : m_sourceGame(nullptr), m_fStateTime(0.0f), m_isReleasingShockwave(false), m_fShockwaveElapsedTime(0.0f), m_fShockwaveCenterX(0.0f), m_fShockwaveCenterY(0.0f), m_hasShownOpeningSequence(false), m_hasOpeningSequenceCompleted(false), m_hasSwiped(false), m_showDeathTransOut(false)
{
    m_json = json;
    m_game = std::unique_ptr<Game>(new Game());
    m_backButton = std::unique_ptr<BackButton>(new BackButton());
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
        
        gs->m_renderer->renderToScreenWorldMapToLevelTransition(m_fTransitionStateTime / 2);
        
        gs->m_renderer->endFrame();
    }
    else
    {
        gs->processTouchEvents();
        
        m_fTransitionStateTime += gs->m_fDeltaTime;
        
        if (m_fTransitionStateTime > 2)
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
