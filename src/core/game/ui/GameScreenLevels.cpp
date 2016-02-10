//
//  GameScreenLevels.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/10/16.
//  Copyright © 2016 Gowen Game Dev. All rights reserved.
//

#include "GameScreenLevels.h"
#include "State.h"
#include "GameScreen.h"
#include "EntityUtils.h"
#include "Vector2D.h"
#include "Game.h"
#include "GameScreenLevelEditor.h"

/// Level ///

Level * Level::getInstance()
{
    static Level *instance = new Level(nullptr);
    
    return instance;
}

void Level::enter(GameScreen* gs)
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

void Level::execute(GameScreen* gs)
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
        
        if (m_hasOpeningSequenceCompleted)
        {
            gs->m_renderer->renderHud(*m_game, *m_backButton, gs->m_iFPS);
        }
        
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
            if (m_activateRadialBlur)
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
            if (gs->m_stateMachine->getPreviousState() == GameScreenLevelEditor::getInstance() && handleOpeningSequenceTouchInput(gs))
            {
                gs->m_renderer->zoomIn();
                m_hasOpeningSequenceCompleted = true;
                jon.setAllowedToMove(m_hasOpeningSequenceCompleted);
                return;
            }
            else
            {
                gs->processTouchEvents();
            }
            
            jon.update(gs->m_fDeltaTime);
            
            int result = gs->m_renderer->updateCameraToFollowPathToJon(*m_game, gs->m_fDeltaTime);
            m_hasOpeningSequenceCompleted = result == 3;
            m_activateRadialBlur = result == 1;
            jon.setAllowedToMove(m_hasOpeningSequenceCompleted);
            if (result == 2)
            {
                jon.beginWarmingUp();
            }
            
            EntityUtils::updateBackgrounds(m_game->getBackgroundUppers(), gs->m_renderer->getCameraPosition(), 0);
            EntityUtils::updateBackgrounds(m_game->getBackgroundMids(), gs->m_renderer->getCameraPosition(), 0);
            EntityUtils::updateBackgrounds(m_game->getBackgroundLowers(), gs->m_renderer->getCameraPosition(), 0);
            EntityUtils::updateBackgrounds(m_game->getBackgroundMidgroundCovers(), gs->m_renderer->getCameraPosition(), 0);
        }
        else
        {
            if (handleTouchInput(gs))
            {
                return;
            }
            
            if (m_showDeathTransOut)
            {
                // Starting new game after death
                
                m_fStateTime += gs->m_fDeltaTime * 2;
                
                if (m_fStateTime > 2.4f)
                {
                    m_fStateTime = 0;
                    m_showDeathTransOut = false;
                }
            }
            
            if (jon.isDead())
            {
                // Starting death transition, when screen goes black, new game begins
                
                m_fStateTime += gs->m_fDeltaTime * 2;
                
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
                
                EntityUtils::handleCollections(jon, m_game->getCollectibleItems(), gs->m_fDeltaTime);
                
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
            
            EntityUtils::updateBackgrounds(m_game->getBackgroundUppers(), gs->m_renderer->getCameraPosition(), gs->m_fDeltaTime);
            EntityUtils::updateBackgrounds(m_game->getBackgroundMids(), gs->m_renderer->getCameraPosition(), gs->m_fDeltaTime);
            EntityUtils::updateBackgrounds(m_game->getBackgroundLowers(), gs->m_renderer->getCameraPosition(), gs->m_fDeltaTime);
            EntityUtils::updateBackgrounds(m_game->getBackgroundMidgroundCovers(), gs->m_renderer->getCameraPosition(), gs->m_fDeltaTime);
        }
    }
}

void Level::exit(GameScreen* gs)
{
    m_game->reset();
    
    Assets::getInstance()->addSoundIdToPlayQueue(SOUND_STOP_JON_VAMPIRE_GLIDE);
    
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

void Level::setSourceGame(Game* game)
{
    m_sourceGame = game;
}

Game& Level::getGame()
{
    return *m_game;
}

BackButton& Level::getBackButton()
{
    return *m_backButton;
}

bool Level::handleOpeningSequenceTouchInput(GameScreen* gs)
{
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
                return true;
        }
    }
    
    return false;
}

bool Level::handleTouchInput(GameScreen* gs)
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

Level::Level(const char* json) : m_sourceGame(nullptr), m_fStateTime(0.0f), m_isReleasingShockwave(false), m_fShockwaveElapsedTime(0.0f), m_fShockwaveCenterX(0.0f), m_fShockwaveCenterY(0.0f), m_hasShownOpeningSequence(false), m_hasOpeningSequenceCompleted(false), m_activateRadialBlur(false), m_hasSwiped(false), m_showDeathTransOut(false)
{
    m_json = json;
    m_game = std::unique_ptr<Game>(new Game());
    m_backButton = std::unique_ptr<BackButton>(new BackButton());
}

/// Chapter 1 Level 1 ///

Chapter1Level1 * Chapter1Level1::getInstance()
{
    static Chapter1Level1 *instance = new Chapter1Level1("{\"jons\":[{\"gridX\":200,\"gridY\":200}]}");
    
    return instance;
}

/// Chapter 1 Level 2 ///

Chapter1Level2 * Chapter1Level2::getInstance()
{
    static Chapter1Level2 *instance = new Chapter1Level2("{\"jons\":[{\"gridX\":200,\"gridY\":200}]}");
    
    return instance;
}

/// Chapter 1 Level 3 ///

Chapter1Level3 * Chapter1Level3::getInstance()
{
    static Chapter1Level3 *instance = new Chapter1Level3("{\"jons\":[{\"gridX\":200,\"gridY\":200}]}");
    
    return instance;
}
