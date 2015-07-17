//
//  GameScreen.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "GameScreen.h"
#include "TouchEvent.h"
#include "Vector2D.h"
#include "Renderer.h"
#include "SpriteBatcher.h"
#include "Circle.h"
#include "ResourceConstants.h"
#include "Rectangle.h"
#include "Assets.h"

GameScreen::GameScreen()
{
    init();
}

void GameScreen::init()
{
    m_touchPoint = std::unique_ptr<Vector2D>(new Vector2D());
    
    m_gameState = RUNNING;
    m_iScreenState = 0; // TODO
}

void GameScreen::onResume()
{
	if(m_gameState == RUNNING)
	{
        Assets::getInstance()->setMusicId(MUSIC_PLAY_DEMO);
        
        platformResume();
	}
}

void GameScreen::onPause()
{
    m_gameState = Game_State::PAUSED;
    
    Assets::getInstance()->setMusicId(MUSIC_STOP);
    
    platformPause();
}

void GameScreen::update(float deltaTime, std::vector<TouchEvent> &touchEvents)
{
    // TODO
}

void GameScreen::present()
{
    m_renderer->clearScreenWithColor(0, 0, 0, 1);
    
    m_renderer->beginFrame();
    
    m_renderer->renderBackground();
    
    m_renderer->endFrame();
}

int GameScreen::getState()
{
	return m_iScreenState;
}

void GameScreen::clearState()
{
    m_iScreenState = 0; // TODO
}