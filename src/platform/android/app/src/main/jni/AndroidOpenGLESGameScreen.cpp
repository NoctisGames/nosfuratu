//
//  AndroidOpenGLESGameScreen.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 7/22/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "AndroidOpenGLESGameScreen.h"
#include "TouchEvent.h"
#include "Vector2D.h"
#include "OpenGLESRenderer.h"
#include "OpenGLESManager.h"
#include "GameConstants.h"
#include "SpriteBatcher.h"
#include "Rectangle.h"
#include "Circle.h"

AndroidOpenGLESGameScreen::AndroidOpenGLESGameScreen() : OpenGLESGameScreen()
{
    // Empty
}

void AndroidOpenGLESGameScreen::touchToWorld(TouchEvent &touchEvent)
{
    m_touchPoint->set((touchEvent.getX() / (float) m_iDeviceScreenWidth) * SCREEN_WIDTH, (1 - touchEvent.getY() / (float) m_iDeviceScreenHeight) * SCREEN_HEIGHT);
}

bool AndroidOpenGLESGameScreen::handleOnBackPressed()
{
    onPause();

    return true;
}