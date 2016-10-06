//
//  IOSOpenGLESGameScreen.h
//  nosfuratu
//
//  Created by Stephen Gowen on 7/22/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__IOSOpenGLESGameScreen__
#define __nosfuratu__IOSOpenGLESGameScreen__

#include "OpenGLESGameScreen.h"

class IOSOpenGLESGameScreen : public OpenGLESGameScreen
{
public:
    IOSOpenGLESGameScreen(int screenWidth, int screenHeight, int pointsWidth, int pointsHeight, bool isLowMemoryDevice);
    
    virtual void touchToWorld(TouchEvent &touchEvent);
    
protected:
    int m_iPointsWidth;
    int m_iPointsHeight;
};

#endif /* defined(__nosfuratu__IOSOpenGLESGameScreen__) */
