//
//  OpenGLESGameScreen.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "OpenGLESGameScreen.h"
#include "TouchEvent.h"
#include "Vector2D.h"
#include "OpenGLESRenderer.h"
#include "OpenGLESManager.h"
#include "GameConstants.h"
#include "SpriteBatcher.h"
#include "Rectangle.h"
#include "Circle.h"

OpenGLESGameScreen::OpenGLESGameScreen() : GameScreen()
{
    // Empty
}

void OpenGLESGameScreen::onSurfaceCreated(int width, int height)
{
	m_iDeviceScreenWidth = width;
	m_iDeviceScreenHeight = height;
    
    OGLESManager->init(width, height);
    
    m_renderer = std::unique_ptr<OpenGLESRenderer>(new OpenGLESRenderer());
}

void OpenGLESGameScreen::onSurfaceChanged(int width, int height)
{
	m_iDeviceScreenWidth = width;
	m_iDeviceScreenHeight = height;
}

void OpenGLESGameScreen::platformResume()
{
    // Empty
}

void OpenGLESGameScreen::platformPause()
{
    // Empty
}

bool OpenGLESGameScreen::handleOnBackPressed()
{
    // Override this if necessary
    // By default we will assume that the platform does not have a back button or does not want to be notified of a back button press
    return false;
}
