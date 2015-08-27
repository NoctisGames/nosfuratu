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

class TouchEvent;
class Vector2D;
class Renderer;

class GameScreen
{
public:
	GameScreen();
    
    virtual void touchToWorld(TouchEvent &touchEvent) = 0;
    
    virtual void platformResume() = 0;
    
    virtual void platformPause() = 0;
    
    void init();

	void onResume();
	
	void onPause();
    
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
    int m_iDeviceScreenWidth;
	int m_iDeviceScreenHeight;
	int m_iScreenState;
    
private:
    std::vector<TouchEvent> m_touchEvents;
    std::vector<TouchEvent> m_touchEventsPool;
    std::vector<TouchEvent> m_touchEventsBuffer;
    
    TouchEvent newTouchEvent();
    
    void addTouchEventForType(Touch_Type touchType, float x, float y);
};

#endif /* GAME_SCREEN_H */