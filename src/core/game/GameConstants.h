//
//  GameConstants.h
//  nosfuratu
//
//  Created by Stephen Gowen on 5/13/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

//// Music Definitions ////

#define MUSIC_STOP 1
#define MUSIC_PLAY_DEMO 2

//// Sound Definitions ////

#define SOUND_COLLECT_CARROT 1
#define SOUND_COLLECT_GOLDEN_CARROT 2

//// Matrix Definitions ////

#define CAM_WIDTH 16.0f
#define CAM_HEIGHT 9.0f

//// Touch Input Definitions ////

#define SWIPE_WIDTH CAM_WIDTH / 32
#define SWIPE_HEIGHT CAM_HEIGHT / 18

//// World Definitions ////

#define GAME_HEIGHT 27.0f

#define GRAVITY -20.0f

#define JON_STARTING_X CAM_WIDTH / 5
#define JON_DEFAULT_MAX_SPEED 7.0f
#define JON_DEFAULT_ACCELERATION 5.0f

#define ANCHOR_GROUND_Y 9.031195840554593f
#define ANCHOR_CAVE_Y 1.216637781629116f

#endif /* GAME_CONSTANTS_H */
