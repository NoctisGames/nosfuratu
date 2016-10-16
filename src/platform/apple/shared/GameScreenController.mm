//
//  GameScreenController.m
//  nosfuratu
//
//  Created by Stephen Gowen on 10/14/16.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#import "GameScreenController.h"

#import "ObjectAL.h"

@interface GameScreenController ()
{
    GameScreen *_gameScreen;
    DisplayMessageBlock _displayMessageBlock;
    GetLevelFilePath _getLevelFilePath;
    NSArray *_soundFileNames;
    NSMutableArray *_playingSounds;
    NSMutableArray *_playingSoundsFileNames;
}

@end

@implementation GameScreenController

- (instancetype)initWithGameScreen:(GameScreen *)gameScreen getLevelFilePath:(GetLevelFilePath)getLevelFilePath andDisplayMessageBlock:(DisplayMessageBlock)displayMessageBlock
{
    self = [super init];
    
    if (self)
    {
        _gameScreen = gameScreen;
        _displayMessageBlock = displayMessageBlock;
        _getLevelFilePath = getLevelFilePath;
        
        [self initSoundEngine];
    }
    
    return self;
}

- (void)dealloc
{
    // Stop all music and sound effects.
    [[OALSimpleAudio sharedInstance] stopEverything];
    
    // Unload all sound effects and bg music so that it doesn't fill
    // memory unnecessarily.
    [[OALSimpleAudio sharedInstance] unloadAllEffects];
}

- (void)update:(float)deltaTime
{
    int requestedAction = _gameScreen->getRequestedAction();
    if (requestedAction >= 1000)
    {
        requestedAction /= 1000;
    }
    
    switch (requestedAction)
    {
        case REQUESTED_ACTION_UPDATE:
            _gameScreen->update(deltaTime);
            break;
        case REQUESTED_ACTION_LEVEL_EDITOR_SAVE:
            [self saveLevel:_gameScreen->getRequestedAction()];
            _gameScreen->clearRequestedAction();
            break;
        case REQUESTED_ACTION_LEVEL_EDITOR_LOAD:
            [self loadLevel:_gameScreen->getRequestedAction()];
            _gameScreen->clearRequestedAction();
            break;
        case REQUESTED_ACTION_LEVEL_COMPLETED:
            [self markLevelAsCompleted:_gameScreen->getRequestedAction()];
            _gameScreen->clearRequestedAction();
            break;
        case REQUESTED_ACTION_SUBMIT_SCORE_ONLINE:
            [self submitScoreOnline:_gameScreen->getRequestedAction()];
            _gameScreen->clearRequestedAction();
            break;
        case REQUESTED_ACTION_UNLOCK_LEVEL:
            [self unlockLevel:_gameScreen->getRequestedAction()];
            _gameScreen->clearRequestedAction();
            break;
        case REQUESTED_ACTION_SET_CUTSCENE_VIEWED:
            [self setCutsceneViewedFlag:_gameScreen->getRequestedAction()];
            _gameScreen->clearRequestedAction();
            break;
        case REQUESTED_ACTION_GET_SAVE_DATA:
            [self sendSaveData];
            _gameScreen->clearRequestedAction();
            break;
        case REQUESTED_ACTION_SHOW_MESSAGE:
            [self showMessage:_gameScreen->getRequestedAction()];
            _gameScreen->clearRequestedAction();
            break;
        default:
            break;
    }
}

- (void)present
{
    _gameScreen->render();
    
    [self handleSound];
    [self handleMusic];
}

- (void)resume
{
    _gameScreen->onResume();
}

- (void)pause
{
    _gameScreen->onPause();
    
    [self pauseMusic];
}

#pragma mark Private

- (void)handleSound
{
    short soundId;
    while ((soundId = _gameScreen->getCurrentSoundId()) > 0)
    {
        switch (soundId)
        {
            case SOUND_JON_VAMPIRE_GLIDE:
            case SOUND_SPARROW_FLY:
            case SOUND_SAW_GRIND:
            case SOUND_SPIKED_BALL_ROLLING:
                [self playSound:soundId isLooping:true];
                break;
            case STOP_SOUND_JON_VAMPIRE_GLIDE:
            case STOP_SOUND_SPARROW_FLY:
            case STOP_SOUND_SAW_GRIND:
            case STOP_SOUND_SPIKED_BALL_ROLLING:
                [self stopSound:soundId - 1000];
                break;
            default:
                [self playSound:soundId];
                break;
        }
    }
}

- (void)handleMusic
{
    short rawMusicId = _gameScreen->getCurrentMusicId();
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
        case MUSIC_SET_VOLUME:
        {
            float volume = rawMusicId / 100.0f / 2.0f; // On iOS, volume starts off at 0.5
            if (volume < 0)
            {
                volume = 0;
            }
            
            [self setMusicVolume:volume];
        }
            break;
        case MUSIC_PLAY_TITLE_LOOP:
            [self playMusic:@"title_bgm.wav" isLooping:YES];
            break;
        case MUSIC_PLAY_LEVEL_SELECT_LOOP:
            [self playMusic:@"level_select_bgm.wav" isLooping:YES];
            break;
        case MUSIC_PLAY_WORLD_1_LOOP:
            [self playMusic:@"world_1_bgm.wav" isLooping:YES];
            break;
        case MUSIC_PLAY_MID_BOSS_LOOP:
            [self playMusic:@"mid_boss_bgm.wav" isLooping:YES];
            break;
        case MUSIC_PLAY_END_BOSS_LOOP:
            [self playMusic:@"final_boss_bgm.wav" isLooping:YES];
            break;
        case MUSIC_PLAY_OPENING_CUTSCENE:
            [self playMusic:@"opening_cutscene_bgm.wav" isLooping:NO];
            break;
        default:
            break;
    }
}

- (void)playMusic:(NSString *)fileName isLooping:(BOOL)isLooping
{
    if ([[OALSimpleAudio sharedInstance] bgPlaying])
    {
        [[OALSimpleAudio sharedInstance] stopBg];
    }
    
    [self setMusicVolume:0.5f];
    [[OALSimpleAudio sharedInstance] playBg:fileName loop:isLooping];
}

- (void)playSound:(int)soundId isLooping:(bool)isLooping
{
    int soundIndex = soundId - 1;
    NSString *soundFileName = [_soundFileNames objectAtIndex:soundIndex];
    id<ALSoundSource> soundSource = [[OALSimpleAudio sharedInstance] playEffect:soundFileName loop:isLooping];
    
    if (isLooping)
    {
        [_playingSounds addObject:soundSource];
        [_playingSoundsFileNames addObject:soundFileName];
    }
}

- (void)playSound:(int)soundId
{
    [self playSound:soundId isLooping:NO];
}

- (void)stopSound:(int)soundId
{
    int soundIndex = soundId - 1;
    NSString *soundFileName = [_soundFileNames objectAtIndex:soundIndex];
    
    int indexToRemove = -1;
    for (int i = 0; i < [_playingSounds count]; i++)
    {
        if ([_playingSoundsFileNames[i] isEqualToString:soundFileName])
        {
            [((id<ALSoundSource>)_playingSounds[i]) stop];
            indexToRemove = i;
            break;
        }
    }
    
    if (indexToRemove > -1)
    {
        [_playingSounds removeObjectAtIndex:indexToRemove];
        [_playingSoundsFileNames removeObjectAtIndex:indexToRemove];
    }
}

- (void)pauseMusic
{
    [[OALSimpleAudio sharedInstance] setBgPaused:true];
}

- (void)resumeMusic
{
    [[OALSimpleAudio sharedInstance] setBgPaused:false];
}

- (void)setMusicVolume:(float)volume
{
    [[OALSimpleAudio sharedInstance] setBgVolume:volume];
}

- (void)saveLevel:(int)requestedAction
{
    NSString* levelFileName = [self getLevelName:requestedAction];
    
    bool result = false;
    const char *level_json = GameScreenLevelEditor::getInstance()->save();
    
    if (level_json)
    {
        NSString *json = [[NSString alloc] initWithCString:level_json encoding:NSUTF8StringEncoding];
        
        NSString *filePath = _getLevelFilePath(levelFileName);
        
        NSError* error;
        [json writeToFile:filePath atomically:YES encoding:NSUTF8StringEncoding error:&error];
        
        result = error ? false : true;
    }
    
    [self displayMessage:result ? @"Level saved successfully" : @"Error occurred while saving level... Please try again!"];
}

- (void)loadLevel:(int)requestedAction
{
    NSString* levelFileName = [self getLevelName:requestedAction];
    
    bool success = false;
    
    NSString *filePath = _getLevelFilePath(levelFileName);
    
    NSError* error;
    NSString *content = [NSString stringWithContentsOfFile:filePath encoding:NSUTF8StringEncoding error:&error];
    
    if (error)
    {
        success = false;
    }
    else
    {
        const char* contentCString = [content cStringUsingEncoding:NSUTF8StringEncoding];
        success = true;
        
        GameScreenLevelEditor::getInstance()->load(contentCString, _gameScreen);
    }
    
    [self displayMessage:success ? @"Level loaded successfully" : @"Error occurred while loading level..."];
}

- (void)unlockLevel:(int)requestedAction
{
    int world = [self calcWorld:requestedAction];
    int level = [self calcLevel:requestedAction];
    int levelStatsFlag = _gameScreen->getLevelStatsFlagForUnlockedLevel();
    int numGoldenCarrots = _gameScreen->getNumGoldenCarrotsAfterUnlockingLevel();
    
    [SaveData setLevelStatsFlag:world level:level levelStatsFlag:levelStatsFlag];
    
    [SaveData setNumGoldenCarrots:numGoldenCarrots];
}

- (void)markLevelAsCompleted:(int)requestedAction
{
    int world = [self calcWorld:requestedAction];
    int level = [self calcLevel:requestedAction];
    int score = _gameScreen->getScore();
    int levelStatsFlag = _gameScreen->getLevelStatsFlag();
    int numGoldenCarrots = _gameScreen->getNumGoldenCarrots();
    int jonUnlockedAbilitiesFlag = _gameScreen->getJonAbilityFlag();
    
    [SaveData setLevelComplete:world level:level score:score levelStatsFlag:levelStatsFlag jonUnlockedAbilitiesFlag:jonUnlockedAbilitiesFlag];
    
    [SaveData setNumGoldenCarrots:numGoldenCarrots];
}

- (void)submitScoreOnline:(int)requestedAction
{
    int world = [self calcWorld:requestedAction];
    int level = [self calcLevel:requestedAction];
    int onlineScore = _gameScreen->getOnlineScore();
    
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

- (void)showMessage:(int)requestedAction
{
    while (requestedAction >= 1000)
    {
        requestedAction -= 1000;
    }
    
    int messageKey = requestedAction;
    
    NSString* toast = nil;
    
    switch (messageKey) {
        case MESSAGE_NO_END_SIGN_KEY:
            toast = [[NSString alloc] initWithCString:MESSAGE_NO_END_SIGN_VAL encoding:NSUTF8StringEncoding];
            break;
        case MESSAGE_NO_JON_KEY:
            toast = [[NSString alloc] initWithCString:MESSAGE_NO_JON_VAL encoding:NSUTF8StringEncoding];
            break;
        case MESSAGE_INVALID_JON_KEY:
            toast = [[NSString alloc] initWithCString:MESSAGE_INVALID_JON_VAL encoding:NSUTF8StringEncoding];
            break;
        case MESSAGE_NO_COUNT_HISS_KEY:
            toast = [[NSString alloc] initWithCString:MESSAGE_NO_COUNT_HISS_VAL encoding:NSUTF8StringEncoding];
            break;
        case MESSAGE_INVALID_COUNT_HISS_KEY:
            toast = [[NSString alloc] initWithCString:MESSAGE_INVALID_COUNT_HISS_VAL encoding:NSUTF8StringEncoding];
            break;
        case MESSAGE_OFFSET_NEEDS_MARKERS_KEY:
            toast = [[NSString alloc] initWithCString:MESSAGE_OFFSET_NEEDS_MARKERS_VAL encoding:NSUTF8StringEncoding];
            break;
        case MESSAGE_FEATURE_COMING_SOON_KEY:
            toast = [[NSString alloc] initWithCString:MESSAGE_FEATURE_COMING_SOON_VAL encoding:NSUTF8StringEncoding];
            break;
        default:
            break;
    }
    
    if (toast)
    {
        [self displayMessage:toast];
    }
}

- (void)displayMessage:(NSString *)message
{
    dispatch_async(dispatch_get_main_queue(), ^
    {
        _displayMessageBlock(message);
    });
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
    _soundFileNames = [NSArray arrayWithObjects:
                       @"collect_carrot.wav",
                       @"collect_golden_carrot.wav",
                       @"death.wav",
                       @"footstep_left_grass.wav",
                       @"footstep_right_grass.wav",
                       @"footstep_left_cave.wav",
                       @"footstep_right_cave.wav",
                       @"jump_spring.wav",
                       @"landing_grass.wav",
                       @"landing_cave.wav",
                       @"snake_death.wav",
                       @"trigger_transform.wav",
                       @"cancel_transform.wav",
                       @"complete_transform.wav",
                       @"jump_spring_heavy.wav",
                       @"jon_rabbit_jump.wav",
                       @"jon_vampire_jump.wav",
                       @"jon_rabbit_double_jump.wav",
                       @"jon_vampire_double_jump.wav",
                       @"vampire_glide_loop.wav",
                       @"mushroom_bounce.wav",
                       @"jon_burrow_rocksfall.wav",
                       @"sparrow_fly_loop.wav",
                       @"sparrow_die.wav",
                       @"toad_die.wav",
                       @"toad_eat.wav",
                       @"saw_grind_loop.wav",
                       @"fox_bounced_on.wav",
                       @"fox_strike.wav",
                       @"fox_death.wav",
                       @"world_1_bgm_intro.wav",
                       @"mid_boss_bgm_intro.wav",
                       @"mid_boss_owl_swoop.wav",
                       @"mid_boss_owl_tree_smash.wav",
                       @"mid_boss_owl_death.wav",
                       @"screen_transition.wav",
                       @"screen_transition_2.wav",
                       @"level_complete.wav",
                       @"title_lightning_1.wav",
                       @"title_lightning_2.wav",
                       @"ability_unlock.wav",
                       @"boss_level_clear.wav",
                       @"level_clear.wav",
                       @"level_selected.wav",
                       @"rabbit_drill.wav",
                       @"snake_jump.wav",
                       @"vampire_dash.wav",
                       @"boss_level_unlock.wav",
                       @"rabbit_stomp.wav",
                       @"final_boss_bgm_intro.wav",
                       @"button_click.wav",
                       @"level_confirmed.wav",
                       @"bat_poof.wav",
                       @"chain_snap.wav",
                       @"end_boss_snake_mouth_open.wav",
                       @"end_boss_snake_charge_cue.wav",
                       @"end_boss_snake_charge.wav",
                       @"end_boss_snake_damaged.wav",
                       @"end_boss_snake_death.wav",
                       @"spiked_ball_rolling_loop.wav",
                       @"absorb_dash_ability.wav", nil];
    
    [OALSimpleAudio sharedInstance].allowIpod = NO;
    
    // Do NOT Mute all audio if the silent switch is turned on.
    [OALSimpleAudio sharedInstance].honorSilentSwitch = NO;
    
    for (NSString *soundFileName in _soundFileNames)
    {
        [[OALSimpleAudio sharedInstance] preloadEffect:soundFileName];
    }
    
    _playingSounds = [[NSMutableArray alloc] init];
    _playingSoundsFileNames = [[NSMutableArray alloc] init];
}

@end
