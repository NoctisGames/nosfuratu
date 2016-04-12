//
//  OpenGLESGameScreen.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "OpenGLESGameScreen.h"

OpenGLESGameScreen::OpenGLESGameScreen() : GameScreen()
{
    // Empty
}

void OpenGLESGameScreen::init(int screenWidth, int screenHeight)
{
    OGLESManager->init(screenWidth, screenHeight, MAX_BATCH_SIZE, NUM_FRAMEBUFFERS);
    
    Assets::getInstance()->setUsingCompressedTextureSet(OGLESManager->m_iMaxTextureSize < 4096 || screenWidth < 1024 || screenHeight < 1024);
    
    if (!m_renderer)
    {
        m_renderer = std::unique_ptr<OpenGLESRenderer>(new OpenGLESRenderer());
    }
    
    m_stateMachine->getCurrentState()->enter(this);
}