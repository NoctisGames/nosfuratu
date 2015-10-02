//
//  OpenGLESGameScreen.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "OpenGLESGameScreen.h"

OpenGLESGameScreen::OpenGLESGameScreen(bool isLevelEditor) : GameScreen(isLevelEditor)
{
    // Empty
}

void OpenGLESGameScreen::initGraphics(int screenWidth, int screenHeight)
{
    OGLESManager->init(screenWidth, screenHeight);
    
    m_renderer = std::unique_ptr<OpenGLESRenderer>(new OpenGLESRenderer());
    
    init();
}