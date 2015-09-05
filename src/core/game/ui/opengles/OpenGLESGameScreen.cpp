//
//  OpenGLESGameScreen.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "OpenGLESGameScreen.h"

OpenGLESGameScreen::OpenGLESGameScreen() : GameScreen()
{
    // Empty
}

void OpenGLESGameScreen::initGraphics(int screenWidth, int screenHeight)
{
    OGLESManager->init(screenWidth, screenHeight, CAM_WIDTH, CAM_HEIGHT);
    
    m_renderer = std::unique_ptr<OpenGLESRenderer>(new OpenGLESRenderer());
}

void OpenGLESGameScreen::platformResume()
{
    // Empty
}

void OpenGLESGameScreen::platformPause()
{
    // Empty
}
