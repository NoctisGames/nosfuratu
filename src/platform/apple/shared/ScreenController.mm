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
#include "MainScreenLevelEditor.h"
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
        _appleSoundManager = [[AppleSoundManager alloc] init];
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
            [self saveLevel:_screen->getRequestedAction()];
            _screen->clearRequestedAction();
            break;
        case REQUESTED_ACTION_LEVEL_EDITOR_LOAD:
            [self loadLevel:_screen->getRequestedAction()];
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
    
    [_appleSoundManager pauseMusic];
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
                [_appleSoundManager playSound:[self soundIndexForSoundId:soundId] volume:1.0f isLooping:true];
                break;
            case STOP_SOUND_JON_VAMPIRE_GLIDE:
            case STOP_SOUND_SPARROW_FLY:
            case STOP_SOUND_SAW_GRIND:
            case STOP_SOUND_SPIKED_BALL_ROLLING:
                [_appleSoundManager stopSound:[self soundIndexForSoundId:(soundId - 1000)]];
                break;
            case RESUME_ALL_SOUNDS:
                [_appleSoundManager resumeAllSounds];
                break;
            case PAUSE_ALL_SOUNDS:
                [_appleSoundManager pauseAllSounds];
                break;
            case STOP_ALL_SOUNDS:
                [_appleSoundManager stopAllSounds];
                break;
            case STOP_ALL_LOOPING_SOUNDS:
                [_appleSoundManager stopAllLoopingSounds];
                break;
            default:
                [_appleSoundManager playSound:[self soundIndexForSoundId:soundId] volume:1.0f isLooping:false];
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
                [_appleSoundManager pauseMusic];
                break;
            case MUSIC_RESUME:
                [_appleSoundManager resumeMusic];
                break;
            case MUSIC_PLAY:
                [_appleSoundManager playMusic:1.0f isLooping:false];
                break;
            case MUSIC_PLAY_LOOP:
                [_appleSoundManager playMusic:1.0f isLooping:true];
                break;
            case MUSIC_SET_VOLUME:
            {
                float volume = rawMusicId / 100.0f;
                
                [_appleSoundManager setMusicVolume:volume];
            }
                break;
            case MUSIC_LOAD_OPENING_CUTSCENE:
                [_appleSoundManager loadMusic:@"opening_cutscene_bgm"];
                break;
            case MUSIC_LOAD_TITLE_LOOP:
                [_appleSoundManager loadMusic:@"title_bgm"];
                break;
            case MUSIC_LOAD_LEVEL_SELECT_LOOP:
                [_appleSoundManager loadMusic:@"level_select_bgm"];
                break;
            case MUSIC_LOAD_WORLD_1_LOOP:
                [_appleSoundManager loadMusic:@"world_1_bgm"];
                break;
            case MUSIC_LOAD_MID_BOSS_LOOP:
                [_appleSoundManager loadMusic:@"mid_boss_bgm"];
                break;
            case MUSIC_LOAD_END_BOSS_LOOP:
                [_appleSoundManager loadMusic:@"final_boss_bgm"];
                break;
            default:
                break;
        }
    }
}

- (int)soundIndexForSoundId:(int)soundId
{
    int soundIndex = soundId - 1;
    
    return soundIndex;
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
    
    int numWorlds = 1;
    for (int i = 1; i <= numWorlds; i++)
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
        if (i < numWorlds)
        {
            usd = [usd stringByAppendingString:@","];
        }
    }
    usd = [usd stringByAppendingString:@"}"];
    
    const char* usdCString = [usd cStringUsingEncoding:NSUTF8StringEncoding];
    
    WorldMap::getInstance()->loadUserSaveData(usdCString);
}

- (void)saveLevel:(int)requestedAction
{
    NSString* levelFileName = [self getLevelName:requestedAction];
    
    const char *level_json = MainScreenLevelEditor::getInstance()->save();
    
    if (level_json)
    {
        NSString *json = [[NSString alloc] initWithCString:level_json encoding:NSUTF8StringEncoding];
        
        NSString *filePath = [NSHomeDirectory() stringByAppendingPathComponent:@"Dropbox/Documents/freelance/NoctisGames/github/nosfuratu-levels"];
        filePath = [filePath stringByAppendingPathComponent:levelFileName];
        
        NSError* error;
        [json writeToFile:filePath atomically:YES encoding:NSUTF8StringEncoding error:&error];
        
        MainScreenLevelEditor::getInstance()->setMessage(error ? "Error Saving Level..." : "Level Saved Successfully!");
    }
}

- (void)loadLevel:(int)requestedAction
{
    NSString* levelFileName = [self getLevelName:requestedAction];
    
    NSString *filePath = [NSHomeDirectory() stringByAppendingPathComponent:@"Dropbox/Documents/freelance/NoctisGames/github/nosfuratu-levels"];
    filePath = [filePath stringByAppendingPathComponent:levelFileName];
    
    NSError* error;
    NSString *content = [NSString stringWithContentsOfFile:filePath encoding:NSUTF8StringEncoding error:&error];
    
    if (error)
    {
        MainScreenLevelEditor::getInstance()->setMessage("Error occurred while loading level...");
    }
    else
    {
        const char* contentCString = [content cStringUsingEncoding:NSUTF8StringEncoding];
        
        MainScreenLevelEditor::getInstance()->load(contentCString, _screen);
    }
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

@end
