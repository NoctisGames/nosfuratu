//
//  MacOpenGLGameScreen.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 7/22/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "MacOpenGLGameScreen.h"

MacOpenGLGameScreen::MacOpenGLGameScreen(int screenWidth, int screenHeight) : OpenGLESGameScreen(false)
{
    m_iPointsWidth = screenWidth;
    m_iPointsHeight = screenHeight;
    
    init(screenWidth, screenHeight);
}

void MacOpenGLGameScreen::touchToWorld(TouchEvent &touchEvent)
{
    m_touchPoint->set((touchEvent.getX() / (float) m_iPointsWidth) * CAM_WIDTH, (touchEvent.getY() / (float) m_iPointsHeight) * CAM_HEIGHT);
}

void MacOpenGLGameScreen::onResize(int screenWidth, int screenHeight)
{
    m_iPointsWidth = screenWidth;
    m_iPointsHeight = screenHeight;
    
    if (m_renderer)
    {
        m_renderer->cleanUp();
        OGLESManager->cleanUp();
    }
    
    init(screenWidth, screenHeight);
}
