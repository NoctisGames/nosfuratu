//
//  OpenGLESGameScreen.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "OpenGLESGameScreen.h"

OpenGLESGameScreen::OpenGLESGameScreen(bool isUsingCompressedTextureSet, bool isLevelEditor) : GameScreen(isLevelEditor)
{
    Assets::getInstance()->setUsingCompressedTextureSet(isUsingCompressedTextureSet);
}