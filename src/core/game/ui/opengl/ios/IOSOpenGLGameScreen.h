//
//  IOSOpenGLGameScreen.h
//  nosfuratu
//
//  Created by Stephen Gowen on 7/22/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__IOSOpenGLGameScreen__
#define __nosfuratu__IOSOpenGLGameScreen__

#include "OpenGLGameScreen.h"

class IOSOpenGLGameScreen : public OpenGLGameScreen
{
public:
    IOSOpenGLGameScreen(int screenWidth, int screenHeight, int pointsWidth, int pointsHeight, bool isLowMemoryDevice);
    
    virtual void touchToWorld(ScreenEvent &touchEvent);
    
protected:
    int m_iPointsWidth;
    int m_iPointsHeight;
};

#endif /* defined(__nosfuratu__IOSOpenGLGameScreen__) */
