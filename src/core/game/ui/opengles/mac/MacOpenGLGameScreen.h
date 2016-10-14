//
//  MacOpenGLGameScreen.h
//  nosfuratu
//
//  Created by Stephen Gowen on 7/22/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__MacOpenGLGameScreen__
#define __nosfuratu__MacOpenGLGameScreen__

#include "OpenGLESGameScreen.h"

class MacOpenGLGameScreen : public OpenGLESGameScreen
{
public:
    MacOpenGLGameScreen(int screenWidth, int screenHeight);
    
    virtual void touchToWorld(TouchEvent &touchEvent);
    
    void onResize(int screenWidth, int screenHeight);
    
protected:
    int m_iPointsWidth;
    int m_iPointsHeight;
};

#endif /* defined(__nosfuratu__MacOpenGLGameScreen__) */
