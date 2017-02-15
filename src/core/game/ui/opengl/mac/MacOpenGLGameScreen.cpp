//
//  MacOpenGLGameScreen.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 7/22/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "MacOpenGLGameScreen.h"

MacOpenGLGameScreen::MacOpenGLGameScreen() : OpenGLGameScreen(false, true)
{
    // Empty
}

void MacOpenGLGameScreen::touchToWorld(ScreenEvent &touchEvent)
{
    m_touchPoint->set((touchEvent.getX() / (float) m_iScreenWidth) * CAM_WIDTH, (touchEvent.getY() / (float) m_iScreenHeight) * CAM_HEIGHT);
}

void MacOpenGLGameScreen::onResize(int screenWidth, int screenHeight)
{
    m_iScreenWidth = screenWidth;
    m_iScreenHeight = screenHeight;
    
    bool needsToEnter = true;
    if (m_renderer)
    {
        needsToEnter = false;
    }
    
    cleanUp();
    
    init(screenWidth, screenHeight);
    
    if (needsToEnter)
    {
        m_stateMachine->getCurrentState()->enter(this);
    }
    else
    {
        GameScreenState* gameScreenState = (GameScreenState*) m_stateMachine->getCurrentState();
        gameScreenState->initRenderer(this);
    }
}

void MacOpenGLGameScreen::cleanUp()
{
    if (m_renderer)
    {
        m_renderer->cleanUp();
        OGLESManager->cleanUp();
    }
}
