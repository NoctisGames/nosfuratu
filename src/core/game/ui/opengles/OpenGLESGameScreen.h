//
//  OpenGLESGameScreen.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__OpenGLESGameScreen__
#define __nosfuratu__OpenGLESGameScreen__

#include "GameScreen.h"
#include "OpenGLESRenderer.h"
#include "OpenGLESManager.h"

class OpenGLESRenderer;

class OpenGLESGameScreen : public GameScreen
{
public:
    OpenGLESGameScreen();
    
protected:
    void initGraphics(int screenWidth, int screenHeight);
    
    virtual void platformResume();
    
    virtual void platformPause();
};

#endif /* defined(__nosfuratu__OpenGLESGameScreen__) */
