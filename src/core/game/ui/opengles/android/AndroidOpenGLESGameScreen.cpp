//
//  AndroidOpenGLESGameScreen.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 7/22/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "AndroidOpenGLESGameScreen.h"

AndroidOpenGLESGameScreen::AndroidOpenGLESGameScreen(bool isLevelEditor) : OpenGLESGameScreen(isLevelEditor)
{
    // Empty
}

void AndroidOpenGLESGameScreen::onSurfaceChanged(int screenWidth, int screenHeight)
{
    m_iScreenWidth = screenWidth;
    m_iScreenHeight = screenHeight;

    initGraphics(screenWidth, screenHeight);
}

void AndroidOpenGLESGameScreen::touchToWorld(TouchEvent &touchEvent)
{
    m_touchPoint->set((touchEvent.getX() / (float) m_iScreenWidth) * CAM_WIDTH, (1 - touchEvent.getY() / (float) m_iScreenHeight) * CAM_HEIGHT);
}

bool AndroidOpenGLESGameScreen::handleOnBackPressed()
{
    onPause();

    return false;
}