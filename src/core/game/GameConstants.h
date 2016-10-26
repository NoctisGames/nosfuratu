//
//  GameConstants.h
//  nosfuratu
//
//  Created by Stephen Gowen on 5/13/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

//// VERSION ////

#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define VERSION_BUILD 3

//// Requested Action Definitions ////

#define REQUESTED_ACTION_UPDATE 0
// Save, Load, Completed, Submit Score Online, and Unlock Level actions are passed in this format: [1-4][1-5][01-21], where the first digit is the action, second is the world, third is the level
#define REQUESTED_ACTION_LEVEL_EDITOR_SAVE 1
#define REQUESTED_ACTION_LEVEL_EDITOR_LOAD 2
#define REQUESTED_ACTION_LEVEL_COMPLETED 3
#define REQUESTED_ACTION_SUBMIT_SCORE_ONLINE 4
#define REQUESTED_ACTION_UNLOCK_LEVEL 5

// Set Cutscene Viewed action is passed in this format: [6][001-999], where the first digit is the action, and the rest is the cutscenes viewed flag
#define REQUESTED_ACTION_SET_CUTSCENE_VIEWED 6

#define REQUESTED_ACTION_GET_SAVE_DATA 7

#define REQUESTED_ACTION_SHOW_MESSAGE 8 // Passed in this format: [8][001-999], where the first digit is the action and the rest determines the actual message (defined below)

//// Message Definitions ////

#define MESSAGE_NO_END_SIGN_KEY 1
#define MESSAGE_NO_END_SIGN_VAL "Cannot save or test a level that does not contain an End Sign"
#define MESSAGE_NO_JON_KEY 2
#define MESSAGE_NO_JON_VAL "Cannot save or test a level that does not contain a Jon"
#define MESSAGE_INVALID_JON_KEY 3
#define MESSAGE_INVALID_JON_VAL "Cannot save or test a level unless Jon is placed to the left of the end sign"
#define MESSAGE_NO_COUNT_HISS_KEY 4
#define MESSAGE_NO_COUNT_HISS_VAL "Cannot save or test a level that does not contain a Count Hiss"
#define MESSAGE_INVALID_COUNT_HISS_KEY 5
#define MESSAGE_INVALID_COUNT_HISS_VAL "Cannot save or test a level unless Count Hiss is placed to the left of the end sign"
#define MESSAGE_OFFSET_NEEDS_MARKERS_KEY 6
#define MESSAGE_OFFSET_NEEDS_MARKERS_VAL "2 markers must be placed to denote the section which you want to offset"
#define MESSAGE_FEATURE_COMING_SOON_KEY 7
#define MESSAGE_FEATURE_COMING_SOON_VAL "This feature is not available yet, but is coming soon!"

//// Music Definitions ////

#define MUSIC_STOP 1
#define MUSIC_RESUME 2
#define MUSIC_PLAY 3 // Assuming music is loaded
#define MUSIC_PLAY_LOOP 4 // Assuming music is loaded
#define MUSIC_SET_VOLUME 5 // Passed in this format: [3][0-100], where the first digit is the action and the rest determines the volume (0-100)
#define MUSIC_LOAD_TITLE_LOOP 6
#define MUSIC_LOAD_OPENING_CUTSCENE 7
#define MUSIC_LOAD_LEVEL_SELECT_LOOP 8
#define MUSIC_LOAD_WORLD_1_LOOP 9
#define MUSIC_LOAD_MID_BOSS_LOOP 10
#define MUSIC_LOAD_END_BOSS_LOOP 11

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
#define SOUND_SNAKE_DEATH 11
#define SOUND_TRIGGER_TRANSFORM 12
#define SOUND_CANCEL_TRANSFORM 13
#define SOUND_COMPLETE_TRANSFORM 14
#define SOUND_JUMP_SPRING_HEAVY 15
#define SOUND_JON_RABBIT_JUMP 16
#define SOUND_JON_VAMPIRE_JUMP 17
#define SOUND_JON_RABBIT_DOUBLE_JUMP 18
#define SOUND_JON_VAMPIRE_DOUBLE_JUMP 19
#define SOUND_JON_VAMPIRE_GLIDE 20
#define STOP_SOUND_JON_VAMPIRE_GLIDE SOUND_JON_VAMPIRE_GLIDE + 1000
#define SOUND_MUSHROOM_BOUNCE 21
#define SOUND_JON_BURROW_ROCKSFALL 22
#define SOUND_SPARROW_FLY 23
#define STOP_SOUND_SPARROW_FLY SOUND_SPARROW_FLY + 1000
#define SOUND_SPARROW_DEATH 24
#define SOUND_TOAD_DEATH 25
#define SOUND_TOAD_EAT 26
#define SOUND_SAW_GRIND 27
#define STOP_SOUND_SAW_GRIND SOUND_SAW_GRIND + 1000
#define SOUND_FOX_BOUNCED_ON 28
#define SOUND_FOX_STRIKE 29
#define SOUND_FOX_DEATH 30
#define SOUND_WORLD_1_LOOP_INTRO 31
#define SOUND_MID_BOSS_LOOP_INTRO 32
#define SOUND_MID_BOSS_SWOOP_DOWN 33
#define SOUND_MID_BOSS_TREE_SMASH 34
#define SOUND_MID_BOSS_DEATH 35
#define SOUND_SCREEN_TRANSITION 36
#define SOUND_SCREEN_TRANSITION_2 37
#define SOUND_LEVEL_COMPLETE 38
#define SOUND_TITLE_LIGHTNING_1 39
#define SOUND_TITLE_LIGHTNING_2 40
#define SOUND_ABILITY_UNLOCK 41
#define SOUND_BOSS_LEVEL_CLEAR 42
#define SOUND_LEVEL_CLEAR 43
#define SOUND_LEVEL_SELECTED 44
#define SOUND_RABBIT_DRILL 45
#define SOUND_SNAKE_JUMP 46
#define SOUND_VAMPIRE_DASH 47
#define SOUND_BOSS_LEVEL_UNLOCK 48
#define SOUND_RABBIT_STOMP 49
#define SOUND_END_BOSS_LOOP_INTRO 50
#define SOUND_BUTTON_CLICK 51
#define SOUND_LEVEL_CONFIRMED 52
#define SOUND_BAT_POOF 53
#define SOUND_CHAIN_SNAP 54
#define SOUND_END_BOSS_SNAKE_MOUTH_OPEN 55
#define SOUND_END_BOSS_SNAKE_CHARGE_CUE 56
#define SOUND_END_BOSS_SNAKE_CHARGE 57
#define SOUND_END_BOSS_SNAKE_DAMAGED 58
#define SOUND_END_BOSS_SNAKE_DEATH 59
#define SOUND_SPIKED_BALL_ROLLING 60
#define STOP_SOUND_SPIKED_BALL_ROLLING SOUND_SPIKED_BALL_ROLLING + 1000
#define SOUND_ABSORB_DASH_ABILITY 61

//// Render Definitions ////

#define MAX_BATCH_SIZE 4096

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

#define GRID_CELL_SIZE 0.140625f

#define ZOOMED_OUT_CAM_WIDTH 63.5625f
#define GAME_HEIGHT 36.0f

#define GAME_GRAVITY -18.0f

#define RABBIT_DEFAULT_MAX_SPEED 7.5f
#define RABBIT_DEFAULT_ACCELERATION 5.0f
#define VAMP_DEFAULT_MAX_SPEED 11.0f
#define VAMP_DEFAULT_ACCELERATION 7.0f

#define SCORE_ENEMY 2500
#define SCORE_CARROT 250
#define SCORE_GOLDEN_CARROT 10000

//// Flag Level Definitions ////

#define FLAG_LEVEL_COMPLETE 1
#define FLAG_FIRST_GOLDEN_CARROT_COLLECTED 2
#define FLAG_SECOND_GOLDEN_CARROT_COLLECTED 4
#define FLAG_THIRD_GOLDEN_CARROT_COLLECTED 8
#define FLAG_BONUS_GOLDEN_CARROT_COLLECTED 16
#define FLAG_LEVEL_UNLOCKED 32

//// Flag Ability Definitions ////

#define FLAG_ABILITY_RABBIT_RIGHT 1
#define FLAG_ABILITY_RABBIT_UP 2
#define FLAG_ABILITY_RABBIT_LEFT 4
#define FLAG_ABILITY_RABBIT_DOWN 8
#define FLAG_ABILITY_VAMPIRE_RIGHT 16
#define FLAG_ABILITY_VAMPIRE_UP 32
#define FLAG_ABILITY_VAMPIRE_LEFT 64
#define FLAG_ABILITY_VAMPIRE_DOWN 128
#define FLAG_ABILITY_DOUBLE_JUMP 256
#define FLAG_ABILITY_TRANSFORM 512

#define FLAG_ABILITY_ALL 1023

//// Flag Cutscene Definitions ////

#define FLAG_CUTSCENE_VIEWED_OPENING 1

//// Render Definitions ////

#define NUM_FRAMEBUFFERS 3

#endif /* GAME_CONSTANTS_H */
