//
//  OpenGLESGameScreen.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__OpenGLESGameScreen__
#define __nosfuratu__OpenGLESGameScreen__

#include "GameScreen.h"
#include "OpenGLESRenderer.h"
#include "OpenGLESManager.h"
#include "OpenGLESTextureGpuProgramWrapper.h"

class OpenGLESGameScreen : public GameScreen
{
public:
    OpenGLESGameScreen(bool isLowMemoryDevice);
    
protected:
    void init(int screenWidth, int screenHeight);
    
private:
    bool m_isLowMemoryDevice;
};

#endif /* defined(__nosfuratu__OpenGLESGameScreen__) */
