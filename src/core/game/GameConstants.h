//
//  GameConstants.h
//  nosfuratu
//
//  Created by Stephen Gowen on 5/13/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

//// Requested Action Definitions ////

#define REQUESTED_ACTION_UPDATE 0
// Save/Load actions are passed in this format: [1-2][1-5][0-20], where the first digit is the action, second is the world, third is the level
#define REQUESTED_ACTION_LEVEL_EDITOR_SAVE 1
#define REQUESTED_ACTION_LEVEL_EDITOR_LOAD 2

//// Music Definitions ////

#define MUSIC_STOP 1
#define MUSIC_RESUME 2
#define MUSIC_PLAY_DEMO 3

//// Sound Definitions ////

#define SOUND_COLLECT_CARROT 1
#define SOUND_COLLECT_GOLDEN_CARROT 2
#define SOUND_DEATH 3
#define SOUND_FOOTSTEP_LEFT_GRASS 4
#define SOUND_FOOTSTEP_RIGHT_GRASS 5
#define SOUND_FOOTSTEP_LEFT_CAVE 6
#define SOUND_FOOTSTEP_RIGHT_CAVE 7
#define SOUND_JUMP_SPRING 8
#define SOUND_LANDING_GRASS 9
#define SOUND_LANDING_CAVE 10
#define SOUND_BREAK_LOG 11
#define SOUND_DESTROY_ROCK 12
#define SOUND_SNAKE_DEATH 13
#define SOUND_TRIGGER_TRANSFORM 14
#define SOUND_CANCEL_TRANSFORM 15
#define SOUND_COMPLETE_TRANSFORM 16
#define SOUND_JUMP_SPRING_HEAVY 17
#define SOUND_JON_RABBIT_JUMP 18
#define SOUND_JON_VAMPIRE_JUMP 19
#define SOUND_JON_RABBIT_DOUBLE_JUMP 20
#define SOUND_JON_VAMPIRE_DOUBLE_JUMP 21
#define SOUND_JON_VAMPIRE_GLIDE 22
#define SOUND_STOP_JON_VAMPIRE_GLIDE 1021

//// Render Definitions ////

#define MAX_BATCH_SIZE 2048

//// Texture Definitions ////

#define TEXTURE_SIZE_2048 2048
#define PIXEL_WIDTH_FOR_BACKGROUND 684.0f

//// Matrix Definitions ////

#define CAM_WIDTH 16.0f
#define CAM_HEIGHT 9.0f

//// Touch Input Definitions ////

#define SWIPE_WIDTH CAM_WIDTH / 32
#define SWIPE_HEIGHT CAM_HEIGHT / 18

//// World Definitions ////

#define ZOOMED_OUT_CAM_WIDTH 48.0f
#define GAME_HEIGHT 27.0f

#define RABBIT_GRAVITY -18.0f
#define VAMP_GRAVITY -20.0f

#define JON_STARTING_X CAM_WIDTH / 5
#define RABBIT_DEFAULT_MAX_SPEED 7.0f
#define RABBIT_DEFAULT_ACCELERATION 4.0f
#define VAMP_DEFAULT_MAX_SPEED 9.0f
#define VAMP_DEFAULT_ACCELERATION 6.0f

//// Render Definitions ////

#define NUM_FRAMEBUFFERS 2

#endif /* GAME_CONSTANTS_H */
