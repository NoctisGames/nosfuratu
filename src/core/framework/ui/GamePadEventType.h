//
//  GamePadEventType.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 1/21/17.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__GamePadEventType__
#define __noctisgames__GamePadEventType__

typedef enum
{
    GamePadEventType_DPAD,
    GamePadEventType_STICK_LEFT,
	GamePadEventType_STICK_RIGHT,
    GamePadEventType_TRIGGER,
    GamePadEventType_BUMPER,
	GamePadEventType_A_BUTTON,
    GamePadEventType_B_BUTTON,
    GamePadEventType_X_BUTTON,
    GamePadEventType_Y_BUTTON,
	GamePadEventType_START_BUTTON,
    GamePadEventType_BACK_BUTTON
} GamePadEventType;

#endif /* defined(__noctisgames__GamePadEventType__) */
