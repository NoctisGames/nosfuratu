//
//  AndroidOpenGLESGameScreen.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 7/22/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "AndroidOpenGLESGameScreen.h"

AndroidOpenGLESGameScreen::AndroidOpenGLESGameScreen(bool isLowMemoryDevice) : OpenGLESGameScreen(isLowMemoryDevice)
{
    // Empty
}

void AndroidOpenGLESGameScreen::onSurfaceCreated()
{
    // Empty
}

void AndroidOpenGLESGameScreen::onSurfaceChanged(int screenWidth, int screenHeight)
{
    m_iScreenWidth = screenWidth;
    m_iScreenHeight = screenHeight;

    if (m_renderer)
    {
        m_renderer->cleanUp();
        OGLESManager->cleanUp();
    }
    
    init(screenWidth, screenHeight);
}

void AndroidOpenGLESGameScreen::touchToWorld(TouchEvent &touchEvent)
{
    m_touchPoint->set((touchEvent.getX() / (float) m_iScreenWidth) * CAM_WIDTH, (1 - touchEvent.getY() / (float) m_iScreenHeight) * CAM_HEIGHT);
}

bool AndroidOpenGLESGameScreen::handleOnBackPressed()
{
    if (m_stateMachine->getPreviousState())
    {
        onTouch(Touch_Type::UP, m_iScreenWidth / 20, m_iScreenHeight / 20);
        return true;
    }
    
    return false;
}
