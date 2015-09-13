//
//  GameScreen.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include <memory>
#include <vector>

#include "TouchType.h"
#include "TouchEvent.h"
#include "Vector2D.h"
#include "SpriteBatcher.h"
#include "Rectangle.h"
#include "Renderer.h"
#include "Circle.h"
#include "ResourceConstants.h"
#include "Assets.h"
#include "GameConstants.h"
#include "Game.h"
#include "Jon.h"

class GameScreen
{
public:
	GameScreen();
    
    virtual void touchToWorld(TouchEvent &touchEvent) = 0;
    
    virtual void init();

	virtual void onResume();
	
	virtual void onPause();
    
	void update(float deltaTime);
    
    void render();
    
    int getState();
    
    void clearState();
    
    short getCurrentMusicId();
    
    short getCurrentSoundId();
    
    void onTouch(Touch_Type type, float raw_touch_x, float raw_touch_y);

protected:
    std::unique_ptr<Renderer> m_renderer;
	std::unique_ptr<Vector2D> m_touchPoint;
    std::unique_ptr<Game> m_game;
    float m_fDeltaTime;
    
private:
    std::vector<TouchEvent> m_touchEvents;
    std::vector<TouchEvent> m_touchEventsPool;
    std::vector<TouchEvent> m_touchEventsBuffer;
    
    TouchEvent newTouchEvent();
    
    void addTouchEventForType(Touch_Type touchType, float x, float y);
};

#endif /* GAME_SCREEN_H */