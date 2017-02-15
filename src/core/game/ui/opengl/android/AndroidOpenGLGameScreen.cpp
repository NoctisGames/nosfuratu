//
//  AndroidOpenGLGameScreen.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 7/22/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "AndroidOpenGLGameScreen.h"

AndroidOpenGLGameScreen::AndroidOpenGLGameScreen(bool isLowMemoryDevice) : OpenGLGameScreen(isLowMemoryDevice)
{
    // Empty
}

void AndroidOpenGLGameScreen::onSurfaceCreated()
{
    // Empty
}

void AndroidOpenGLGameScreen::onSurfaceChanged(int screenWidth, int screenHeight)
{
    m_iScreenWidth = screenWidth;
    m_iScreenHeight = screenHeight;
    
    bool onResize = false;
    if (m_renderer)
    {
        m_renderer->cleanUp();
        OGLESManager->cleanUp();
        
        onResize = true;
    }
    
    init(screenWidth, screenHeight, onResize);
    
    if (onResize)
    {
        GameScreenState* gameScreenState = (GameScreenState*) m_stateMachine->getCurrentState();
        gameScreenState->initRenderer(this);
    }
}

void AndroidOpenGLGameScreen::touchToWorld(ScreenEvent &touchEvent)
{
    m_touchPoint->set((touchEvent.getX() / (float) m_iScreenWidth) * CAM_WIDTH, (1 - touchEvent.getY() / (float) m_iScreenHeight) * CAM_HEIGHT);
}

bool AndroidOpenGLGameScreen::handleOnBackPressed()
{
    if (m_stateMachine->getPreviousState())
    {
        onTouch(ScreenEventType_UP, m_iScreenWidth / 20, m_iScreenHeight / 20);
        return true;
    }
    
    return false;
}
