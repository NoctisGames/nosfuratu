//
//  IOSOpenGLESGameScreen.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 7/22/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "IOSOpenGLESGameScreen.h"

#ifdef GGD_LEVEL_EDITOR
#define IS_LEVEL_EDITOR true
#else
#define IS_LEVEL_EDITOR false
#endif

IOSOpenGLESGameScreen::IOSOpenGLESGameScreen(int screenWidth, int screenHeight, int pointsWidth, int pointsHeight, bool useCompressedTextureSet) : OpenGLESGameScreen(useCompressedTextureSet, IS_LEVEL_EDITOR)
{
    m_iPointsWidth = pointsWidth;
    m_iPointsHeight = pointsHeight;
    
    OGLESManager->init(screenWidth, screenHeight, MAX_BATCH_SIZE, NUM_FRAMEBUFFERS);
    
    m_renderer = std::unique_ptr<OpenGLESRenderer>(new OpenGLESRenderer());
    
    m_stateMachine->getCurrentState()->enter(this);
}

void IOSOpenGLESGameScreen::touchToWorld(TouchEvent &touchEvent)
{
    m_touchPoint->set((touchEvent.getX() / (float) m_iPointsWidth) * CAM_WIDTH, (1 - touchEvent.getY() / (float) m_iPointsHeight) * CAM_HEIGHT);
}