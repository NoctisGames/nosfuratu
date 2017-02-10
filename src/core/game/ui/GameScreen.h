//
//  GameScreen.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include <memory>
#include <vector>

#include "TouchType.h"
#include "TouchEvent.h"
#include "Vector2D.h"
#include "SpriteBatcher.h"
#include "RectangleBatcher.h"
#include "Rectangle.h"
#include "Renderer.h"
#include "Circle.h"
#include "Assets.h"
#include "AssetsMapper.h"
#include "GameConstants.h"
#include "Jon.h"
#include "StateMachine.h"
#include "GpuProgramWrapper.h"
#include "SinWaveTextureGpuProgramWrapper.h"
#include "SnakeDeathTextureGpuProgramWrapper.h"
#include "ShockwaveTextureGpuProgramWrapper.h"
#include "BatPanel.h"
#include "StateMachine.h"
#include "GameScreenState.h"
#include "RTTI.h"

class GameHudCarrot : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    GameHudCarrot(bool isGolden) : PhysicalEntity(0, 0, 1, 1), m_isGolden(isGolden) {}
    
    bool isGolden() { return m_isGolden; }
    
private:
    bool m_isGolden;
};

#include "State.h"
#include "GameButton.h"
#include "TitlePanel.h"

#include <memory>

class GameScreen
{
    RTTI_DECL;
    
public:
    std::unique_ptr<StateMachine<GameScreen, GameScreenState>> m_stateMachine;
    std::vector<TouchEvent *> m_touchEvents;
    std::vector<TouchEvent *> m_touchEventsPool;
    std::vector<TouchEvent *> m_touchEventsBuffer;
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<Vector2D> m_touchPoint;
    std::unique_ptr<Vector2D> m_touchPointDown;
    std::unique_ptr<Vector2D> m_touchPointDown2;
	float m_fFPSStateTime;
    float m_fTimeUntilResume;
	int m_iFrames;
	int m_iFPS;
	float m_fDeltaTime;
    float m_fScreenHeldTime;
    bool m_isRequestingRender;
    int m_iRequestedAction;
    bool m_isPaused;
    bool m_isScreenHeldDown;
    bool m_hasSwiped;
    bool m_isReleasingShockwave;
    bool m_needsToResumeMusicAfterTexLoad;
    float m_fShockwaveCenterX;
    float m_fShockwaveCenterY;
    float m_fShockwaveElapsedTime;
    
	GameScreen();
    
    virtual void touchToWorld(TouchEvent &touchEvent) = 0;

	virtual void onResume();
	
	virtual void onPause();
    
	void update(float deltaTime);
    
    void render();
    
    int getRequestedAction();
    
    void clearRequestedAction();
    
    short getCurrentMusicId();
    
    short getCurrentSoundId();
    
    int getScore();
    
    int getOnlineScore();
    
    int getLevelStatsFlag();
    
    int getLevelStatsFlagForUnlockedLevel();
    
    int getNumGoldenCarrots();
    
    int getNumGoldenCarrotsAfterUnlockingLevel();
    
    int getJonAbilityFlag();
    
    void onTouch(Touch_Type type, float raw_touch_x, float raw_touch_y);
    
    void processTouchEvents();
    
private:
    float m_fFrameStateTime;
    int m_iPoolIndex;
    bool m_wasPaused;
    
    void internalUpdate();

    TouchEvent* newTouchEvent();
    
    void addTouchEventForType(Touch_Type type, float x, float y);
    
    void handleTouchInput();
};

#endif /* GAME_SCREEN_H */
