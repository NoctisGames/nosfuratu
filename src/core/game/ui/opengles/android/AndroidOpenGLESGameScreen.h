//
//  AndroidOpenGLESGameScreen.h
//  nosfuratu
//
//  Created by Stephen Gowen on 7/22/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__AndroidOpenGLESGameScreen__
#define __nosfuratu__AndroidOpenGLESGameScreen__

#include "OpenGLESGameScreen.h"

class AndroidOpenGLESGameScreen : public OpenGLESGameScreen
{
public:
    AndroidOpenGLESGameScreen();

    void onSurfaceChanged(int screenWidth, int screenHeight);

    virtual void touchToWorld(TouchEvent &touchEvent);

    bool handleOnBackPressed();

private:
    int m_iScreenWidth;
    int m_iScreenHeight;
};

#endif /* defined(__nosfuratu__AndroidOpenGLESGameScreen__) */
