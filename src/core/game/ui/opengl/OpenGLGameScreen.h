//
//  OpenGLGameScreen.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__OpenGLGameScreen__
#define __nosfuratu__OpenGLGameScreen__

#include "GameScreen.h"
#include "OpenGLRenderer.h"
#include "OpenGLManager.h"
#include "OpenGLTextureGpuProgramWrapper.h"

class OpenGLGameScreen : public GameScreen
{
public:
    OpenGLGameScreen(bool isLowMemoryDevice, bool isUsingDesktopTextureSet = false);
    
protected:
    void init(int screenWidth, int screenHeight, bool onResize = false);
    
private:
    bool m_isLowMemoryDevice;
    bool m_isUsingDesktopTextureSet;
};

#endif /* defined(__nosfuratu__OpenGLGameScreen__) */
