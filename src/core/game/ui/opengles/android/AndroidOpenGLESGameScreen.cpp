//
//  AndroidOpenGLESGameScreen.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 7/22/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "AndroidOpenGLESGameScreen.h"

AndroidOpenGLESGameScreen::AndroidOpenGLESGameScreen(bool isUsingCompressedTextureSet, bool isLevelEditor) : OpenGLESGameScreen(isUsingCompressedTextureSet, isLevelEditor)
{
    // Empty
}

void AndroidOpenGLESGameScreen::onSurfaceCreated()
{
    if (m_renderer)
    {
        m_renderer->cleanUp();
        m_renderer->reinit();
    }
    else
    {
        m_renderer = std::unique_ptr<OpenGLESRenderer>(new OpenGLESRenderer());
    }
}

void AndroidOpenGLESGameScreen::onSurfaceChanged(int screenWidth, int screenHeight)
{
    m_iScreenWidth = screenWidth;
    m_iScreenHeight = screenHeight;

    OGLESManager->init(screenWidth, screenHeight, MAX_BATCH_SIZE, NUM_FRAMEBUFFERS);
    
    m_stateMachine->getCurrentState()->enter(this);
}

void AndroidOpenGLESGameScreen::touchToWorld(TouchEvent &touchEvent)
{
    m_touchPoint->set((touchEvent.getX() / (float) m_iScreenWidth) * CAM_WIDTH, (1 - touchEvent.getY() / (float) m_iScreenHeight) * CAM_HEIGHT);
}

bool AndroidOpenGLESGameScreen::handleOnBackPressed()
{
    if (m_stateMachine->isInState(*GamePlay::getInstance()))
    {
        onTouch(Touch_Type::UP, m_iScreenWidth / 20, m_iScreenHeight / 20);
        return true;
    }
    
    return false;
}