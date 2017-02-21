//
//  ScreenController.m
//  nosfuratu
//
//  Created by Stephen Gowen on 10/14/16.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#import "ScreenController.h"

#import "AppleSoundManager.h"
#import "SaveData.h"

// C++
#include "GameConstants.h"
#include "MainScreenWorldMap.h"
#include "Game.h"
#include "GameConstants.h"
#include "SoundManager.h"

@interface ScreenController ()
{
    MainScreen *_screen;
    HandleInterstitialAd _handleInterstitialAd;
    AppleSoundManager *_appleSoundManager;
}

@end

@implementation ScreenController

- (instancetype)initWithScreen:(MainScreen *)screen andInterstitialAdHandler:(HandleInterstitialAd)handleInterstitialAd
{
    self = [super init];
    
    if (self)
    {
        _screen = screen;
        _handleInterstitialAd = handleInterstitialAd;
        
        [self initSoundEngine];
    }
    
    return self;
}

- (void)update:(float)deltaTime
{
    int requestedAction = _screen->getRequestedAction();
    if (requestedAction >= 1000)
    {
        requestedAction /= 1000;
    }
    
    switch (requestedAction)
    {
        case REQUESTED_ACTION_LEVEL_EDITOR_SAVE:
            _screen->clearRequestedAction();
            break;
        case REQUESTED_ACTION_LEVEL_EDITOR_LOAD:
            _screen->clearRequestedAction();
            break;
        case REQUESTED_ACTION_LEVEL_COMPLETED:
            [self markLevelAsCompleted:_screen->getRequestedAction()];
            _screen->clearRequestedAction();
            break;
        case REQUESTED_ACTION_SUBMIT_SCORE_ONLINE:
            [self submitScoreOnline:_screen->getRequestedAction()];
            _screen->clearRequestedAction();
            break;
        case REQUESTED_ACTION_UNLOCK_LEVEL:
            [self unlockLevel:_screen->getRequestedAction()];
            _screen->clearRequestedAction();
            break;
        case REQUESTED_ACTION_SET_CUTSCENE_VIEWED:
            [self setCutsceneViewedFlag:_screen->getRequestedAction()];
            _screen->clearRequestedAction();
            break;
        case REQUESTED_ACTION_GET_SAVE_DATA:
            [self sendSaveData];
            _screen->clearRequestedAction();
            break;
        case REQUESTED_ACTION_DISPLAY_INTERSTITIAL_AD:
            if (_handleInterstitialAd)
            {
                _handleInterstitialAd();
            }
            _screen->clearRequestedAction();
            break;
        case REQUESTED_ACTION_UPDATE:
        default:
            break;
    }
    
    _screen->update(deltaTime);
}

- (void)present
{
    _screen->render();
    
    [self handleSound];
    [self handleMusic];
}

- (void)resume
{
    _screen->onResume();
}

- (void)pause
{
    _screen->onPause();
    
    [self pauseMusic];
}

#pragma mark Private

- (void)handleSound
{
    short soundId;
    while ((soundId = SOUND_MANAGER->getCurrentSoundId()) > 0)
    {
        switch (soundId)
        {
            case SOUND_JON_VAMPIRE_GLIDE:
            case SOUND_SPARROW_FLY:
            case SOUND_SAW_GRIND:
            case SOUND_SPIKED_BALL_ROLLING:
                [self playSound:soundId isLooping:YES];
                break;
            case STOP_SOUND_JON_VAMPIRE_GLIDE:
            case STOP_SOUND_SPARROW_FLY:
            case STOP_SOUND_SAW_GRIND:
            case STOP_SOUND_SPIKED_BALL_ROLLING:
                [self stopSound:soundId - 1000];
                break;
            case STOP_ALL_SOUNDS:
                [self stopAllSounds];
                break;
            case STOP_ALL_LOOPING_SOUNDS:
                [self stopAllLoopingSounds];
                break;
            default:
                [self playSound:soundId];
                break;
        }
    }
}

- (void)handleMusic
{
    short rawMusicId;
    while ((rawMusicId = SOUND_MANAGER->getCurrentMusicId()) > 0)
    {
        short musicId = rawMusicId;
        if (musicId >= 1000)
        {
            musicId /= 1000;
            rawMusicId -= musicId * 1000;
        }
        
        switch (musicId)
        {
            case MUSIC_STOP:
                [self pauseMusic];
                break;
            case MUSIC_RESUME:
                [self resumeMusic];
                break;
            case MUSIC_PLAY:
                [self playMusic:NO];
                break;
            case MUSIC_PLAY_LOOP:
                [self playMusic:YES];
                break;
            case MUSIC_SET_VOLUME:
            {
                float volume = rawMusicId / 100.0f;
                
                [self setMusicVolume:volume];
            }
                break;
            case MUSIC_LOAD_OPENING_CUTSCENE:
                [self loadMusic:@"opening_cutscene_bgm"];
                break;
            case MUSIC_LOAD_TITLE_LOOP:
                [self loadMusic:@"title_bgm"];
                break;
            case MUSIC_LOAD_LEVEL_SELECT_LOOP:
                [self loadMusic:@"level_select_bgm"];
                break;
            case MUSIC_LOAD_WORLD_1_LOOP:
                [self loadMusic:@"world_1_bgm"];
                break;
            case MUSIC_LOAD_MID_BOSS_LOOP:
                [self loadMusic:@"mid_boss_bgm"];
                break;
            case MUSIC_LOAD_END_BOSS_LOOP:
                [self loadMusic:@"final_boss_bgm"];
                break;
            default:
                break;
        }
    }
}

- (void)loadMusic:(NSString *)fileName
{
    [_appleSoundManager loadMusic:fileName];
}

- (void)playMusic:(BOOL)isLooping
{
    [_appleSoundManager playMusic:0.5f isLooping:isLooping];
}

- (void)playSound:(int)soundId isLooping:(BOOL)isLooping
{
    int soundIndex = soundId - 1;
    [_appleSoundManager playSound:soundIndex volume:1.0f isLooping:isLooping];
}

- (void)playSound:(int)soundId
{
    [self playSound:soundId isLooping:NO];
}

- (void)stopSound:(int)soundId
{
    int soundIndex = soundId - 1;
    [_appleSoundManager stopSound:soundIndex];
}

- (void)stopAllSounds
{
    [_appleSoundManager stopAllSounds];
}

- (void)stopAllLoopingSounds
{
    [_appleSoundManager stopAllLoopingSounds];
}

- (void)pauseMusic
{
    [_appleSoundManager pauseMusic];
}

- (void)resumeMusic
{
    [_appleSoundManager resumeMusic];
}

- (void)setMusicVolume:(float)volume
{
    [_appleSoundManager setMusicVolume:volume];
}

- (void)unlockLevel:(int)requestedAction
{
    int world = [self calcWorld:requestedAction];
    int level = [self calcLevel:requestedAction];
    int levelStatsFlag = _screen->getLevelStatsFlagForUnlockedLevel();
    int numGoldenCarrots = _screen->getNumGoldenCarrotsAfterUnlockingLevel();
    
    [SaveData setLevelStatsFlag:world level:level levelStatsFlag:levelStatsFlag];
    
    [SaveData setNumGoldenCarrots:numGoldenCarrots];
}

- (void)markLevelAsCompleted:(int)requestedAction
{
    int world = [self calcWorld:requestedAction];
    int level = [self calcLevel:requestedAction];
    int score = _screen->getScore();
    int levelStatsFlag = _screen->getLevelStatsFlag();
    int numGoldenCarrots = _screen->getNumGoldenCarrots();
    int jonUnlockedAbilitiesFlag = _screen->getJonAbilityFlag();
    
    [SaveData setLevelComplete:world level:level score:score levelStatsFlag:levelStatsFlag jonUnlockedAbilitiesFlag:jonUnlockedAbilitiesFlag];
    
    [SaveData setNumGoldenCarrots:numGoldenCarrots];
}

- (void)submitScoreOnline:(int)requestedAction
{
    int world = [self calcWorld:requestedAction];
    int level = [self calcLevel:requestedAction];
    int onlineScore = _screen->getOnlineScore();
    
    // TODO, submit score using game center, on success, save the score that was pushed online
    
    [SaveData setScorePushedOnline:world level:level score:onlineScore];
}

- (void)setCutsceneViewedFlag:(int)requestedAction
{
    while (requestedAction >= 1000)
    {
        requestedAction -= 1000;
    }
    
    int cutsceneViewedFlag = requestedAction;
    
    [SaveData setViewedCutscenesFlag:cutsceneViewedFlag];
}

- (void)sendSaveData
{
    int numGoldenCarrots = [SaveData getNumGoldenCarrots];
    int jonUnlockedAbilitiesFlag = [SaveData getJonUnlockedAbilitiesFlag];
    int viewedCutscenesFlag = [SaveData getViewedCutscenesFlag];
    
    NSString* usd = @"{";
    usd = [usd stringByAppendingFormat:@"\"num_golden_carrots\": %i, ", numGoldenCarrots];
    usd = [usd stringByAppendingFormat:@"\"jon_unlocked_abilities_flag\": %i, ", jonUnlockedAbilitiesFlag];
    usd = [usd stringByAppendingFormat:@"\"viewed_cutscenes_flag\": %i, ", viewedCutscenesFlag];
    
    for (int i = 1; i <= 5; i++)
    {
        usd = [usd stringByAppendingFormat:@"\"world_%i\":[", i];
        for (int j = 1; j <= 21; j++)
        {
            int statsFlag = [SaveData getLevelStatsFlag:i level:j];
            int score = [SaveData getLevelScore:i level:j];
            int scoreOnline = [SaveData getScorePushedOnline:i level:j];
            
            usd = [usd stringByAppendingString:@"{"];
            usd = [usd stringByAppendingFormat:@"\"stats_flag\": %i, ", statsFlag];
            usd = [usd stringByAppendingFormat:@"\"score\": %i, ", score];
            usd = [usd stringByAppendingFormat:@"\"score_online\": %i ", scoreOnline];
            
            usd = [usd stringByAppendingString:@"}"];
            if (j < 21)
            {
                usd = [usd stringByAppendingString:@","];
            }
            
            usd = [usd stringByAppendingString:@" "];
        }
        usd = [usd stringByAppendingString:@"]"];
        if (i < 5)
        {
            usd = [usd stringByAppendingString:@","];
        }
    }
    usd = [usd stringByAppendingString:@"}"];
    
    const char* usdCString = [usd cStringUsingEncoding:NSUTF8StringEncoding];
    
    WorldMap::getInstance()->loadUserSaveData(usdCString);
}

- (NSString *)getLevelName:(int)requestedAction
{
    int world = [self calcWorld:requestedAction];
    int level = [self calcLevel:requestedAction];
    
    if (world > 0 && level > 0)
    {
        return [NSString stringWithFormat:@"nosfuratu_c%i_l%i.json", world, level];
    }
    else
    {
        return @"nosfuratu.json";
    }
}

- (int)calcWorld:(int)requestedAction
{
    int world = 0;
    
    while (requestedAction >= 1000)
    {
        requestedAction -= 1000;
    }
    
    while (requestedAction >= 100)
    {
        requestedAction -= 100;
        world++;
    }
    
    return world;
}

- (int)calcLevel:(int)requestedAction
{
    while (requestedAction >= 1000)
    {
        requestedAction -= 1000;
    }
    
    while (requestedAction >= 100)
    {
        requestedAction -= 100;
    }
    
    int level = requestedAction;
    
    return level;
}

- (void)initSoundEngine
{
    _appleSoundManager = [[AppleSoundManager alloc] init];
    
    [_appleSoundManager loadSound:@"collect_carrot" withNumCopies:6];
    [_appleSoundManager loadSound:@"collect_golden_carrot" withNumCopies:1];
    [_appleSoundManager loadSound:@"death" withNumCopies:1];
    [_appleSoundManager loadSound:@"footstep_left_grass" withNumCopies:1];
    [_appleSoundManager loadSound:@"footstep_right_grass" withNumCopies:1];
    [_appleSoundManager loadSound:@"footstep_left_cave" withNumCopies:1];
    [_appleSoundManager loadSound:@"footstep_right_cave" withNumCopies:1];
    [_appleSoundManager loadSound:@"jump_spring" withNumCopies:1];
    [_appleSoundManager loadSound:@"landing_grass" withNumCopies:1];
    [_appleSoundManager loadSound:@"landing_cave" withNumCopies:1];
    [_appleSoundManager loadSound:@"snake_death" withNumCopies:3];
    [_appleSoundManager loadSound:@"trigger_transform" withNumCopies:2];
    [_appleSoundManager loadSound:@"cancel_transform" withNumCopies:2];
    [_appleSoundManager loadSound:@"complete_transform" withNumCopies:1];
    [_appleSoundManager loadSound:@"jump_spring_heavy" withNumCopies:1];
    [_appleSoundManager loadSound:@"jon_rabbit_jump" withNumCopies:1];
    [_appleSoundManager loadSound:@"jon_vampire_jump" withNumCopies:1];
    [_appleSoundManager loadSound:@"jon_rabbit_double_jump" withNumCopies:2];
    [_appleSoundManager loadSound:@"jon_vampire_double_jump" withNumCopies:2];
    [_appleSoundManager loadSound:@"vampire_glide_loop" withNumCopies:1];
    [_appleSoundManager loadSound:@"mushroom_bounce" withNumCopies:3];
    [_appleSoundManager loadSound:@"jon_burrow_rocksfall" withNumCopies:1];
    [_appleSoundManager loadSound:@"sparrow_fly_loop" withNumCopies:3];
    [_appleSoundManager loadSound:@"sparrow_die" withNumCopies:3];
    [_appleSoundManager loadSound:@"toad_die" withNumCopies:1];
    [_appleSoundManager loadSound:@"toad_eat" withNumCopies:1];
    [_appleSoundManager loadSound:@"saw_grind_loop" withNumCopies:3];
    [_appleSoundManager loadSound:@"fox_bounced_on" withNumCopies:1];
    [_appleSoundManager loadSound:@"fox_strike" withNumCopies:3];
    [_appleSoundManager loadSound:@"fox_death" withNumCopies:3];
    [_appleSoundManager loadSound:@"world_1_bgm_intro" withNumCopies:1];
    [_appleSoundManager loadSound:@"mid_boss_bgm_intro" withNumCopies:1];
    [_appleSoundManager loadSound:@"mid_boss_owl_swoop" withNumCopies:1];
    [_appleSoundManager loadSound:@"mid_boss_owl_tree_smash" withNumCopies:1];
    [_appleSoundManager loadSound:@"mid_boss_owl_death" withNumCopies:1];
    [_appleSoundManager loadSound:@"screen_transition" withNumCopies:1];
    [_appleSoundManager loadSound:@"screen_transition_2" withNumCopies:1];
    [_appleSoundManager loadSound:@"level_complete" withNumCopies:1];
    [_appleSoundManager loadSound:@"title_lightning_1" withNumCopies:2];
    [_appleSoundManager loadSound:@"title_lightning_2" withNumCopies:2];
    [_appleSoundManager loadSound:@"ability_unlock" withNumCopies:1];
    [_appleSoundManager loadSound:@"boss_level_clear" withNumCopies:1];
    [_appleSoundManager loadSound:@"level_clear" withNumCopies:1];
    [_appleSoundManager loadSound:@"level_selected" withNumCopies:3];
    [_appleSoundManager loadSound:@"rabbit_drill" withNumCopies:2];
    [_appleSoundManager loadSound:@"snake_jump" withNumCopies:3];
    [_appleSoundManager loadSound:@"vampire_dash" withNumCopies:3];
    [_appleSoundManager loadSound:@"boss_level_unlock" withNumCopies:1];
    [_appleSoundManager loadSound:@"rabbit_stomp" withNumCopies:3];
    [_appleSoundManager loadSound:@"final_boss_bgm_intro" withNumCopies:1];
    [_appleSoundManager loadSound:@"button_click" withNumCopies:3];
    [_appleSoundManager loadSound:@"level_confirmed" withNumCopies:1];
    [_appleSoundManager loadSound:@"bat_poof" withNumCopies:1];
    [_appleSoundManager loadSound:@"chain_snap" withNumCopies:1];
    [_appleSoundManager loadSound:@"end_boss_snake_mouth_open" withNumCopies:1];
    [_appleSoundManager loadSound:@"end_boss_snake_charge_cue" withNumCopies:1];
    [_appleSoundManager loadSound:@"end_boss_snake_charge" withNumCopies:1];
    [_appleSoundManager loadSound:@"end_boss_snake_damaged" withNumCopies:1];
    [_appleSoundManager loadSound:@"end_boss_snake_death" withNumCopies:1];
    [_appleSoundManager loadSound:@"spiked_ball_rolling_loop" withNumCopies:2];
    [_appleSoundManager loadSound:@"absorb_dash_ability" withNumCopies:1];
    [_appleSoundManager loadSound:@"footstep_left_wood" withNumCopies:1];
    [_appleSoundManager loadSound:@"footstep_right_wood" withNumCopies:1];
    [_appleSoundManager loadSound:@"landing_wood" withNumCopies:1];
}

@end
