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
    
    gs->getGame().update(gs->getDeltaTime());
    gs->getRenderer().updateCameraToFollowJon(gs->getGame().getJon(), gs->getGame(), gs->getDeltaTime());
    
    if (gs->getGame().resetGame())
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
    for (std::vector<TouchEvent>::iterator i = gs->getTouchEvents().begin(); i != gs->getTouchEvents().end(); i++)
    {
        if(gs->getTouchEventsPool().size() < 50)
        {
            gs->getTouchEventsPool().push_back(*i);
        }
    }
    
    gs->getTouchEvents().clear();
    gs->getTouchEvents().swap(gs->getTouchEventsBuffer());
    gs->getTouchEventsBuffer().clear();
    
    for (std::vector<TouchEvent>::iterator i = gs->getTouchEvents().begin(); i != gs->getTouchEvents().end(); i++)
    {
        gs->touchToWorld((*i));
        
        switch (i->getTouchType())
        {
            case DOWN:
                gs->getTouchPointDown().set(gs->getTouchPoint().getX(), gs->getTouchPoint().getY());
                continue;
            case DRAGGED:
                continue;
            case UP:
                if (gs->getTouchPointDown().getX() + SWIPE_WIDTH <= gs->getTouchPoint().getX())
                {
                    // Swipe Right
                    gs->getGame().getJon().triggerRightAction();
                }
                else if (gs->getTouchPointDown().getX() - SWIPE_WIDTH >= gs->getTouchPoint().getX())
                {
                    // Swipe Left
                    gs->getGame().getJon().triggerLeftAction();
                }
                else if (gs->getTouchPointDown().getY() + SWIPE_HEIGHT <= gs->getTouchPoint().getY())
                {
                    // Swipe Up
                    gs->getGame().getJon().triggerUpAction();
                }
                else if (gs->getTouchPointDown().getY() - SWIPE_HEIGHT >= gs->getTouchPoint().getY())
                {
                    // Swipe Down
                    gs->getGame().getJon().triggerDownAction();
                }
                else
                {
                    gs->getGame().getJon().triggerJump();
                }
                
                gs->getTouchPointDown().set(gs->getTouchPoint().getX(), gs->getTouchPoint().getY());
                
                return;
        }
    }
}