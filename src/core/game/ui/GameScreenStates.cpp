//
//  UpwardSpike.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/29/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "GameScreenStates.h"
#include "State.h"
#include "GameScreen.h"
#include "EntityUtils.h"

/// Game Play ///

GamePlay * GamePlay::getInstance()
{
    static GamePlay *instance = new GamePlay();
    
    return instance;
}

void GamePlay::enter(GameScreen* gs)
{
    // TODO
}

void GamePlay::execute(GameScreen* gs)
{
    handleTouchInput(gs);
    
    Jon& jon = gs->m_game->getJon();
    
    jon.update(gs->m_fDeltaTime, *gs->m_game);
    
    if (EntityUtils::isCollected(jon, gs->m_game->getCarrots(), gs->m_fDeltaTime))
    {
        Assets::getInstance()->addSoundIdToPlayQueue(SOUND_COLLECT_CARROT);
    }
    
    if (EntityUtils::isCollected(jon, gs->m_game->getGoldenCarrots(), gs->m_fDeltaTime))
    {
        Assets::getInstance()->addSoundIdToPlayQueue(SOUND_COLLECT_GOLDEN_CARROT);
    }
    
    EntityUtils::update(gs->m_game->getGoldenCarrots(), gs->m_fDeltaTime);
    
    EntityUtils::update(gs->m_game->getJumpSprings(), gs->m_fDeltaTime);
    
    EntityUtils::updateAndClean(gs->m_game->getRocks(), gs->m_fDeltaTime);
    
    gs->m_renderer->updateCameraToFollowJon(jon, *gs->m_game, gs->m_fDeltaTime);
    
    EntityUtils::updateBackgrounds(gs->m_game->getBackgroundSkies(), gs->m_renderer->getCameraPosition());
    EntityUtils::updateBackgrounds(gs->m_game->getBackgroundTrees(), gs->m_renderer->getCameraPosition());
    EntityUtils::updateBackgrounds(gs->m_game->getBackgroundCaves(), gs->m_renderer->getCameraPosition());
    
    if (jon.isDead() || jon.getPosition().getX() - jon.getWidth() > gs->m_game->getFarRight())
    {
        gs->init();
    }
}

void GamePlay::exit(GameScreen* gs)
{
    // TODO
}

void GamePlay::handleTouchInput(GameScreen* gs)
{
    gs->processTouchEvents();
    
    Jon& jon = gs->m_game->getJon();
    
    for (std::vector<TouchEvent>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
    {
        gs->touchToWorld((*i));
        
        switch (i->getTouchType())
        {
            case DOWN:
                gs->m_touchPointDown->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());
                continue;
            case DRAGGED:
                continue;
            case UP:
                if (gs->m_touchPointDown->getX() + SWIPE_WIDTH <= gs->m_touchPoint->getX())
                {
                    // Swipe Right
                    jon.triggerRightAction();
                }
                else if (gs->m_touchPointDown->getX() - SWIPE_WIDTH >= gs->m_touchPoint->getX())
                {
                    // Swipe Left
                    jon.triggerLeftAction();
                }
                else if (gs->m_touchPointDown->getY() + SWIPE_HEIGHT <= gs->m_touchPoint->getY())
                {
                    // Swipe Up
                    jon.triggerUpAction();
                }
                else if (gs->m_touchPointDown->getY() - SWIPE_HEIGHT >= gs->m_touchPoint->getY())
                {
                    // Swipe Down
                    jon.triggerDownAction();
                }
                else
                {
                    jon.triggerJump();
                }
                
                gs->m_touchPointDown->set(gs->m_touchPoint->getX(), gs->m_touchPoint->getY());
                
                return;
        }
    }
}

/// Level Editor ///

LevelEditor * LevelEditor::getInstance()
{
    static LevelEditor *instance = new LevelEditor();
    
    return instance;
}

void LevelEditor::enter(GameScreen* gs)
{
    // TODO
}

void LevelEditor::execute(GameScreen* gs)
{
    handleTouchInput(gs);
    
    Jon& jon = gs->m_game->getJon();
    
    jon.update(gs->m_fDeltaTime, *gs->m_game);
    
    if (EntityUtils::isCollected(jon, gs->m_game->getCarrots(), gs->m_fDeltaTime))
    {
        Assets::getInstance()->addSoundIdToPlayQueue(SOUND_COLLECT_CARROT);
    }
    
    if (EntityUtils::isCollected(jon, gs->m_game->getGoldenCarrots(), gs->m_fDeltaTime))
    {
        Assets::getInstance()->addSoundIdToPlayQueue(SOUND_COLLECT_GOLDEN_CARROT);
    }
    
    EntityUtils::update(gs->m_game->getGoldenCarrots(), gs->m_fDeltaTime);
    
    EntityUtils::update(gs->m_game->getJumpSprings(), gs->m_fDeltaTime);
    
    EntityUtils::updateAndClean(gs->m_game->getRocks(), gs->m_fDeltaTime);
    
    EntityUtils::updateBackgrounds(gs->m_game->getBackgroundSkies(), gs->m_renderer->getCameraPosition());
    EntityUtils::updateBackgrounds(gs->m_game->getBackgroundTrees(), gs->m_renderer->getCameraPosition());
    EntityUtils::updateBackgrounds(gs->m_game->getBackgroundCaves(), gs->m_renderer->getCameraPosition());
}

void LevelEditor::exit(GameScreen* gs)
{
    // TODO
}

void LevelEditor::handleTouchInput(GameScreen* gs)
{
    gs->processTouchEvents();
    
    for (std::vector<TouchEvent>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
    {
        gs->touchToWorld((*i));
        
        switch (i->getTouchType())
        {
            case DOWN:
                // TODO
                continue;
            case DRAGGED:
                continue;
            case UP:
                // TODO
                return;
        }
    }
}