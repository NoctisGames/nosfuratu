//
//  AndroidOpenGLGameScreen.h
//  nosfuratu
//
//  Created by Stephen Gowen on 7/22/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__AndroidOpenGLGameScreen__
#define __nosfuratu__AndroidOpenGLGameScreen__

#include "OpenGLGameScreen.h"

class AndroidOpenGLGameScreen : public OpenGLGameScreen
{
public:
    AndroidOpenGLGameScreen(bool isLowMemoryDevice);

    void onSurfaceCreated();
    
    void onSurfaceChanged(int screenWidth, int screenHeight);

    virtual void touchToWorld(TouchEvent &touchEvent);

    bool handleOnBackPressed();

private:
    int m_iScreenWidth;
    int m_iScreenHeight;
};

#endif /* defined(__nosfuratu__AndroidOpenGLGameScreen__) */
