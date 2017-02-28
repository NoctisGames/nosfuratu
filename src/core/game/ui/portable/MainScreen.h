//
//  MainScreen.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__MainScreen__
#define __nosfuratu__MainScreen__

#include "IScreen.h"

#include "ScreenEventType.h"
#include "ScreenEvent.h"
#include "Vector2D.h"
#include "SpriteBatcher.h"
#include "NGRectBatcher.h"
#include "NGRect.h"
#include "MainRenderer.h"
#include "Circle.h"
#include "Assets.h"
#include "GameConstants.h"
#include "Jon.h"
#include "StateMachine.h"
#include "GpuProgramWrapper.h"
#include "SinWaveTextureGpuProgramWrapper.h"
#include "SnakeDeathTextureGpuProgramWrapper.h"
#include "ShockwaveTextureGpuProgramWrapper.h"
#include "BatPanel.h"
#include "StateMachine.h"
#include "MainScreenState.h"
#include "State.h"
#include "GameButton.h"
#include "TitlePanel.h"
#include "IDeviceHelper.h"
#include "DeviceHelperFactory.h"
#include "RTTI.h"

#include <memory>
#include <vector>

class GameHudItem : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    static GameHudItem createCarrot() { return GameHudItem(0); }
    static GameHudItem createGoldenCarrot() { return GameHudItem(1); }
    static GameHudItem createVial() { return GameHudItem(2); }
    
    GameHudItem(int type) : PhysicalEntity(0, 0, 1, 1), m_iType(type) {}
    
    bool isCarrot() { return m_iType == 0; }
    bool isGolden() { return m_iType == 1; }
    bool isVial() { return m_iType == 2; }
    
private:
    int m_iType;
};

class MainScreen : public IScreen
{
    RTTI_DECL;
    
public:
    IDeviceHelper* m_deviceHelper;
    MainRenderer* m_renderer;
    StateMachine<MainScreen, MainScreenState> m_stateMachine;
    Vector2D m_touchPointDown;
    Vector2D m_touchPointDown2;
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
    bool m_needsToResumeMusicAfterTexLoad;
    bool m_needsToResumeAudio;
    float m_fShockwaveCenterX;
    float m_fShockwaveCenterY;
    float m_fShockwaveElapsedTime;
    
	MainScreen();
    
    virtual ~MainScreen();
    
    virtual void createDeviceDependentResources();
    
    virtual void createWindowSizeDependentResources(int screenWidth, int screenHeight, int touchScreenWidth, int touchScreenHeight);
    
    virtual void releaseDeviceDependentResources();

	virtual void onResume();
	
	virtual void onPause();
    
	virtual void update(float deltaTime);
    
    virtual void render();
    
    int getRequestedAction();
    
    void clearRequestedAction();
    
    int getScore();
    
    int getOnlineScore();
    
    int getLevelStatsFlag();
    
    int getLevelStatsFlagForUnlockedLevel();
    
    int getNumGoldenCarrots();
    
    int getNumGoldenCarrotsAfterUnlockingLevel();
    
    int getJonAbilityFlag();
    
private:
    float m_fFrameStateTime;
    bool m_wasPaused;
    
    void internalUpdate();
};

#endif /* defined(__nosfuratu__MainScreen__) */
