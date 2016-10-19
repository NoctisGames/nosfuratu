//
//  MacOpenGLGameScreen.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 7/22/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "MacOpenGLGameScreen.h"

MacOpenGLGameScreen::MacOpenGLGameScreen(int screenWidth, int screenHeight) : OpenGLGameScreen(false, true)
{
    m_iScreenWidth = screenWidth;
    m_iScreenHeight = screenHeight;
    
    init(screenWidth, screenHeight);
}

void MacOpenGLGameScreen::touchToWorld(TouchEvent &touchEvent)
{
    m_touchPoint->set((touchEvent.getX() / (float) m_iScreenWidth) * CAM_WIDTH, (touchEvent.getY() / (float) m_iScreenHeight) * CAM_HEIGHT);
}

void MacOpenGLGameScreen::onResize(int screenWidth, int screenHeight)
{
    m_iScreenWidth = screenWidth;
    m_iScreenHeight = screenHeight;
    
    cleanUp();
    
    init(screenWidth, screenHeight);
}

void MacOpenGLGameScreen::cleanUp()
{
    if (m_renderer)
    {
        m_renderer->cleanUp();
        OGLESManager->cleanUp();
    }
}
