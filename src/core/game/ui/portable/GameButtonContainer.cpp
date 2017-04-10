//
//  GameButtonContainer.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 4/9/17.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "GameButtonContainer.h"

#include "MainScreen.h"
#include "GameButton.h"

#include "OverlapTester.h"
#include "ScreenEvent.h"
#include "Vector2D.h"
#include "ScreenInputManager.h"
#include "TouchConverter.h"

GameButtonContainer::GameButtonContainer(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height),
m_isAnimating(false), m_isOpening(false)
{
    m_fStateTime = -1;
    
    float w = m_fWidth;
    float h = m_fHeight;
    float l = m_position.getX() - w / 2;
    float b = m_position.getY() - (h / 2);
    
    {
        float iw = w;
        float ih = h * 0.229166666666667f;
        
        m_googlePlayController = new GameButton(l + 0.5f * w, b + h * 0.864583333333333f, iw, ih, GameButtonType_GameController);
        m_googlePlayAchievements = new GameButton(l + 0.5f * w, b + h * 0.614583333333333f, iw, ih, GameButtonType_Achievements);
        m_googlePlayLeaderboards = new GameButton(l + 0.5f * w, b + h * 0.364583333333333f, iw, ih, GameButtonType_Leaderboards);
        m_googlePlaySignInOut = new GameButton(l + 0.5f * w, b + h * 0.114583333333333f, iw, ih, GameButtonType_SignInOut);
    }
    
    m_googlePlayController->setAlt(false);
    m_googlePlayController->setHidden(false);
    m_googlePlayAchievements->setHidden(true);
    m_googlePlayLeaderboards->setHidden(true);
    m_googlePlaySignInOut->setHidden(true);
    m_googlePlaySignInOut->setAlt(false);
}

GameButtonContainer::~GameButtonContainer()
{
    delete m_googlePlayController;
    delete m_googlePlayAchievements;
    delete m_googlePlayLeaderboards;
    delete m_googlePlaySignInOut;
}

void GameButtonContainer::update(float deltaTime)
{
    if (m_isOpening)
    {
        m_fStateTime += deltaTime;
        
        if (m_fStateTime > 0.5f)
        {
            m_googlePlayController->setHidden(false);
            m_googlePlayAchievements->setHidden(false);
            m_googlePlayLeaderboards->setHidden(false);
            m_googlePlaySignInOut->setHidden(false);
            
            m_googlePlayController->setAlt(true);
            m_googlePlaySignInOut->setAlt(m_screen->m_isAuthenticated);
            
            m_isAnimating = false;
        }
    }
    else
    {
        m_fStateTime -= deltaTime;
        
        if (m_fStateTime < 0)
        {
            m_isAnimating = false;
        }
    }
}

int GameButtonContainer::handleInput()
{
    for (std::vector<ScreenEvent *>::iterator i = SCREEN_INPUT_MANAGER->getEvents().begin(); i != SCREEN_INPUT_MANAGER->getEvents().end(); ++i)
    {
        Vector2D& touchPoint = TOUCH_CONVERTER->touchToWorld(*(*i));
        
        switch ((*i)->getType())
        {
            case ScreenEventType_DOWN:
            case ScreenEventType_DRAGGED:
                continue;
            case ScreenEventType_UP:
            {
                if (OverlapTester::isPointInNGRect(touchPoint, getMainBounds()))
                {
                    if (!m_googlePlayController->isHidden()
                        && OverlapTester::isPointInNGRect(touchPoint, m_googlePlayController->getMainBounds()))
                    {
                        if (isClosed())
                        {
                            open();
                        }
                        else
                        {
                            close();
                        }
                        
                        return GAME_BUTTON_CONTAINER_RC_HANDLED;
                    }
                    
                    if (!m_googlePlayAchievements->isHidden()
                        && OverlapTester::isPointInNGRect(touchPoint, m_googlePlayAchievements->getMainBounds()))
                    {
                        return GAME_BUTTON_CONTAINER_RC_SHOW_ACHIEVEMENTS;
                    }
                    
                    if (!m_googlePlayLeaderboards->isHidden()
                        && OverlapTester::isPointInNGRect(touchPoint, m_googlePlayLeaderboards->getMainBounds()))
                    {
                        return GAME_BUTTON_CONTAINER_RC_SHOW_LEADERBOARDS;
                    }
                    
                    if (!m_googlePlaySignInOut->isHidden()
                        && OverlapTester::isPointInNGRect(touchPoint, m_googlePlaySignInOut->getMainBounds()))
                    {
                        return m_screen->m_isAuthenticated ? GAME_BUTTON_CONTAINER_RC_SIGN_OUT : GAME_BUTTON_CONTAINER_RC_SIGN_IN;
                    }
                    
                    return GAME_BUTTON_CONTAINER_RC_HANDLED;
                }
            }
                break;
        }
    }
    
    return GAME_BUTTON_CONTAINER_RC_UNHANDLED;
}

void GameButtonContainer::config(MainScreen* screen)
{
    m_screen = screen;
}

void GameButtonContainer::open()
{
    m_googlePlayController->setHidden(true);
    
    m_fStateTime = 0;
    m_isOpening = true;
    m_isAnimating = true;
}

void GameButtonContainer::close()
{
    m_googlePlayController->setHidden(true);
    m_googlePlayAchievements->setHidden(true);
    m_googlePlayLeaderboards->setHidden(true);
    m_googlePlaySignInOut->setHidden(true);
    
    m_fStateTime = 0.5f;
    m_isOpening = false;
    m_isAnimating = true;
}

bool GameButtonContainer::isAnimating()
{
    return m_isAnimating;
}

bool GameButtonContainer::isOpen()
{
    return !m_isAnimating && m_fStateTime > 0.5f;
}

bool GameButtonContainer::isClosed()
{
    return !m_isAnimating && m_fStateTime < 0;
}

GameButton* GameButtonContainer::getGooglePlayController()
{
    return m_googlePlayController;
}

GameButton* GameButtonContainer::getGooglePlayAchievements()
{
    return m_googlePlayAchievements;
}

GameButton* GameButtonContainer::getGooglePlayLeaderboards()
{
    return m_googlePlayLeaderboards;
}

GameButton* GameButtonContainer::getGooglePlaySignInOut()
{
    return m_googlePlaySignInOut;
}

RTTI_IMPL(GameButtonContainer, PhysicalEntity);
