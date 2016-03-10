//
//  GameConstants.h
//  nosfuratu
//
//  Created by Stephen Gowen on 5/13/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
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

#define NO_SOUND 0
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
#define SOUND_DESTROY_ROCK 11
#define SOUND_SNAKE_DEATH 12
#define SOUND_TRIGGER_TRANSFORM 13
#define SOUND_CANCEL_TRANSFORM 14
#define SOUND_COMPLETE_TRANSFORM 15
#define SOUND_JUMP_SPRING_HEAVY 16
#define SOUND_JON_RABBIT_JUMP 17
#define SOUND_JON_VAMPIRE_JUMP 18
#define SOUND_JON_RABBIT_DOUBLE_JUMP 19
#define SOUND_JON_VAMPIRE_DOUBLE_JUMP 20
#define SOUND_JON_VAMPIRE_GLIDE 21
#define STOP_SOUND_JON_VAMPIRE_GLIDE SOUND_JON_VAMPIRE_GLIDE + 1000
#define SOUND_MUSHROOM_BOUNCE 22
#define SOUND_JON_BURROW_ROCKSFALL 23
#define SOUND_SPARROW_FLY 24
#define STOP_SOUND_SPARROW_FLY SOUND_SPARROW_FLY + 1000
#define SOUND_SPARROW_DEATH 25
#define SOUND_TOAD_DEATH 26
#define SOUND_TOAD_EAT 27
#define SOUND_SAW_GRIND 28
#define STOP_SOUND_SAW_GRIND SOUND_SAW_GRIND + 1000
#define SOUND_FOX_BOUNCED_ON 29
#define SOUND_FOX_STRIKE 30
#define SOUND_FOX_DEATH 31

//// Render Definitions ////

#define MAX_BATCH_SIZE 2048

//// Texture Definitions ////

#define TEXTURE_SIZE_1024 1024
#define TEXTURE_SIZE_2048 2048
#define TEXTURE_SIZE_4096 4096
#define PIXEL_WIDTH_FOR_BACKGROUND 1808

//// Matrix Definitions ////

#define CAM_WIDTH 15.890625f
#define CAM_HEIGHT 9.0f

//// Touch Input Definitions ////

#define SWIPE_WIDTH CAM_WIDTH / 32
#define SWIPE_HEIGHT CAM_HEIGHT / 18

//// World Definitions ////

#define ZOOMED_OUT_CAM_WIDTH 63.5625f
#define GAME_HEIGHT 36.0f

#define RABBIT_GRAVITY -18.0f
#define VAMP_GRAVITY -24.0f

#define JON_STARTING_X CAM_WIDTH / 5
#define RABBIT_DEFAULT_MAX_SPEED 7.2f
#define RABBIT_DEFAULT_ACCELERATION 7.2f
#define VAMP_DEFAULT_MAX_SPEED 9.0f
#define VAMP_DEFAULT_ACCELERATION 9.0f

//// Render Definitions ////

#define NUM_FRAMEBUFFERS 2

#endif /* GAME_CONSTANTS_H */
