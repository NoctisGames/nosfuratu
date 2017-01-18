//
//  MacOpenGLGameScreen.h
//  nosfuratu
//
//  Created by Stephen Gowen on 7/22/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__MacOpenGLGameScreen__
#define __nosfuratu__MacOpenGLGameScreen__

#include "OpenGLGameScreen.h"

class MacOpenGLGameScreen : public OpenGLGameScreen
{
public:
    MacOpenGLGameScreen();
    
    virtual void touchToWorld(TouchEvent &touchEvent);
    
    void onResize(int screenWidth, int screenHeight);
    
    void cleanUp();
    
private:
    int m_iScreenWidth;
    int m_iScreenHeight;
};

#endif /* defined(__nosfuratu__MacOpenGLGameScreen__) */
