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

IOSOpenGLESGameScreen::IOSOpenGLESGameScreen(int screenWidth, int screenHeight, int pointsWidth, int pointsHeight) : OpenGLESGameScreen(IS_LEVEL_EDITOR)
{
    m_iPointsWidth = pointsWidth;
    m_iPointsHeight = pointsHeight;
    
    initGraphics(screenWidth, screenHeight);
}

void IOSOpenGLESGameScreen::touchToWorld(TouchEvent &touchEvent)
{
    m_touchPoint->set((touchEvent.getX() / (float) m_iPointsWidth) * CAM_WIDTH, (((float)m_iPointsHeight - touchEvent.getY()) / (float) m_iPointsHeight) * CAM_HEIGHT);
}