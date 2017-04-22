//
//  MainScreen.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__MainScreen__
#define __nosfuratu__MainScreen__

#include "IScreen.h"

#include "StateMachine.h"
#include "MainScreenState.h"

#include "RTTI.h"

#include <vector>

class JsonFile;
class MainRenderer;
class Vector2D;

class MainScreen : public IScreen
{
    RTTI_DECL;
    
public:
    JsonFile* m_saveData;
    MainRenderer* m_renderer;
    Vector2D* m_touchPointDown;
    Vector2D* m_touchPointDown2;
    StateMachine<MainScreen, MainScreenState> m_stateMachine;
	float m_fFPSStateTime;
    float m_fTimeUntilResume;
	int m_iFrames;
	int m_iFPS;
	float m_fDeltaTime;
    float m_fScreenHeldTime;
    bool m_isRequestingRender;
    int m_iRequestedAction;
    int m_iNumInternalUpdates;
    bool m_isPaused;
    bool m_isScreenHeldDown;
    bool m_hasSwiped;
    bool m_isReleasingShockwave;
    bool m_needsToResumeAudio;
    bool m_isAuthenticated;
    float m_fShockwaveCenterX;
    float m_fShockwaveCenterY;
    float m_fShockwaveElapsedTime;
    
	MainScreen();
    
    virtual ~MainScreen();
    
    virtual void createDeviceDependentResources();
    
    virtual void createWindowSizeDependentResources(int renderWidth, int renderHeight, int touchScreenWidth, int touchScreenHeight);
    
    virtual void releaseDeviceDependentResources();

	virtual void onResume();
	
	virtual void onPause();
    
	virtual void update(float deltaTime);
    
    virtual void render();
    
    void initSounds();
    
    int getRequestedAction();
    
    void clearRequestedAction();
    
    void setAuthenticated(bool isAuthenticated);
    
    int getScore();
    
    std::string getLeaderboardKey();
    
    std::vector<std::string>& getUnlockedAchievementsKeys();
    
private:
    float m_fFrameStateTime;
    bool m_wasPaused;
    
    void internalUpdate();
};

#endif /* defined(__nosfuratu__MainScreen__) */
