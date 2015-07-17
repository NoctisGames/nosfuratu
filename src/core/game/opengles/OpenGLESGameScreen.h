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

class OpenGLESRenderer;

class OpenGLESGameScreen : public GameScreen
{
public:
    OpenGLESGameScreen(bool isRunningIOS8 = false);
    
    void onSurfaceCreated(int deviceScreenWidth, int deviceScreenHeight);
    
	void onSurfaceChanged(int deviceScreenWidth, int deviceScreenHeight, int dpWidth, int dpHeight);
    
    virtual void touchToWorld(TouchEvent &touchEvent);
    
    virtual void platformResume();
    
    virtual void platformPause();
    
    virtual bool handleOnBackPressed();

private:
    int m_iDeviceScreenDpWidth;
    int m_iDeviceScreenDpHeight;
    bool m_isRunningIOS8;
};

#endif /* defined(__nosfuratu__OpenGLESGameScreen__) */
