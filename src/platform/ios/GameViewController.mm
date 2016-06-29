//
//  GameViewController.m
//  nosfuratu
//
//  Created by Stephen Gowen on 9/5/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#import "GameViewController.h"
#import "UIView+Toast.h"
#import "SaveData.h"

// Sound Engine
#import "CMOpenALSoundManager.h"

// C++
#include "IOSOpenGLESGameScreen.h"
#include "GameConstants.h"
#include "GameScreenLevelEditor.h"
#include "GameScreenWorldMap.h"
#include "Game.h"

@interface GameViewController ()
{
    IOSOpenGLESGameScreen *gameScreen;
}

@property (strong, nonatomic) EAGLContext *context;
@property (nonatomic, retain) CMOpenALSoundManager *soundMgr;

@end

@implementation GameViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    if (!self.context)
    {
        NSLog(@"Failed to create ES context");
        return;
    }
    
    GLKView *view = (GLKView *)self.view;
    
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    view.userInteractionEnabled = YES;
    [view setMultipleTouchEnabled:YES];
    
    self.preferredFramesPerSecond = 60;
    
    [EAGLContext setCurrentContext:self.context];
    
    CGRect screenBounds = [[UIScreen mainScreen] bounds];
    CGFloat screenScale = [[UIScreen mainScreen] scale];
    CGSize screenSize = CGSizeMake(screenBounds.size.width * screenScale, screenBounds.size.height * screenScale);
    
    CGSize size = CGSizeMake(screenSize.width, screenSize.height);
    size.width = roundf(size.width);
    size.height = roundf(size.height);
    
    NSLog(@"dimension %f x %f", size.width, size.height);
    
    [view bindDrawable];
    
    [self initSoundEngine];
    
    gameScreen = new IOSOpenGLESGameScreen(MAX(size.width, size.height), MIN(size.width, size.height), [UIScreen mainScreen].applicationFrame.size.width, [UIScreen mainScreen].applicationFrame.size.height);
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(onPause)
                                                 name:UIApplicationWillResignActiveNotification
                                               object:nil];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(onResume)
                                                 name:UIApplicationDidBecomeActiveNotification
                                               object:nil];
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    
    gameScreen->onResume();
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    
    [self onPause];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    CGPoint pos = [touch locationInView: [UIApplication sharedApplication].keyWindow];
    gameScreen->onTouch(DOWN, pos.x, pos.y);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    CGPoint pos = [touch locationInView: [UIApplication sharedApplication].keyWindow];
    gameScreen->onTouch(DRAGGED, pos.x, pos.y);
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    CGPoint pos = [touch locationInView: [UIApplication sharedApplication].keyWindow];
    gameScreen->onTouch(UP, pos.x, pos.y);
}

#pragma mark <GLKViewControllerDelegate>

- (void)update
{
    int requestedAction = gameScreen->getRequestedAction();
    if (requestedAction >= 100000)
    {
        requestedAction /= 100000;
    }
    
    switch (requestedAction)
    {
        case REQUESTED_ACTION_UPDATE:
            gameScreen->update(self.timeSinceLastUpdate);
            break;
        case REQUESTED_ACTION_LEVEL_EDITOR_SAVE:
            [self saveLevel:gameScreen->getRequestedAction()];
            gameScreen->clearRequestedAction();
            break;
        case REQUESTED_ACTION_LEVEL_EDITOR_LOAD:
            [self loadLevel:gameScreen->getRequestedAction()];
            gameScreen->clearRequestedAction();
            break;
        case REQUESTED_ACTION_LEVEL_COMPLETED:
            [self markLevelAsCompleted:gameScreen->getRequestedAction()];
            gameScreen->clearRequestedAction();
            break;
        case REQUESTED_ACTION_GET_LEVEL_STATS:
            [self sendLevelStats];
            gameScreen->clearRequestedAction();
            break;
        case REQUESTED_ACTION_LEVEL_EDITOR_SHOW_MESSAGE:
            [self showMessage:gameScreen->getRequestedAction()];
            gameScreen->clearRequestedAction();
            break;
        default:
            break;
    }
}

#pragma mark <GLKViewDelegate>

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    gameScreen->render();
    [self handleSound];
    [self handleMusic];
}

#pragma mark <Private>

- (void)handleSound
{
    short soundId;
    while ((soundId = gameScreen->getCurrentSoundId()) > 0)
    {
        switch (soundId)
        {
            case SOUND_JON_VAMPIRE_GLIDE:
            case SOUND_SPARROW_FLY:
            case SOUND_SAW_GRIND:
                [self playSound:soundId isLooping:true];
                break;
            case STOP_SOUND_JON_VAMPIRE_GLIDE:
            case STOP_SOUND_SPARROW_FLY:
            case STOP_SOUND_SAW_GRIND:
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
    short rawMusicId = gameScreen->getCurrentMusicId();
    short musicId = rawMusicId;
    if (musicId >= 1000)
    {
        musicId /= 1000;
        rawMusicId -= musicId * 1000;
    }
    
    switch (musicId)
    {
        case MUSIC_STOP:
            [self.soundMgr stopBackgroundMusic];
            break;
        case MUSIC_RESUME:
            [self.soundMgr resumeBackgroundMusic];
            break;
        case MUSIC_SET_VOLUME:
        {
            float volume = rawMusicId / 100.0f;
            if (volume < 0)
            {
                volume = 0;
            }
            
            self.soundMgr.backgroundMusicVolume = volume;
        }
            break;
        case MUSIC_PLAY_WORLD_1_LOOP:
            if ([self.soundMgr isBackGroundMusicPlaying])
            {
                [self.soundMgr stopBackgroundMusic];
            }
            
            self.soundMgr.backgroundMusicVolume = 0.5f;
            [self.soundMgr playBackgroundMusic:@"world_1_bgm.wav" forcePlay:YES isLooping:YES];
            break;
        case MUSIC_PLAY_MID_BOSS_LOOP:
            if ([self.soundMgr isBackGroundMusicPlaying])
            {
                [self.soundMgr stopBackgroundMusic];
            }
            
            self.soundMgr.backgroundMusicVolume = 0.5f;
            [self.soundMgr playBackgroundMusic:@"mid_boss_bgm.wav" forcePlay:YES isLooping:YES];
            break;
        default:
            break;
    }
}

- (void)playSound:(int)soundId isLooping:(bool)isLooping
{
    [self.soundMgr playSoundWithID:soundId - 1 isLooping:isLooping];
}

- (void)playSound:(int)soundId
{
    [self.soundMgr playSoundWithID:soundId - 1];
}

- (void)stopSound:(int)soundId
{
    [self.soundMgr stopSoundWithID:soundId - 1];
}

- (void)saveLevel:(int)requestedAction
{
    NSString* levelFileName = [self getLevelName:requestedAction];
    
    bool result = false;
    const char *level_json = GameScreenLevelEditor::getInstance()->save();
    
    if (level_json)
    {
        NSString *json = [[NSString alloc] initWithCString:level_json encoding:NSUTF8StringEncoding];
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *documentsDirectory = [paths objectAtIndex:0];
        NSString *filePath = [documentsDirectory stringByAppendingPathComponent:levelFileName];
        
        NSError* error;
        [json writeToFile:filePath atomically:YES encoding:NSUTF8StringEncoding error:&error];
        
        result = error ? false : true;
    }
    
    [self.view makeToast:result ? @"Level saved successfully" : @"Error occurred while saving level... Please try again!"];
}

- (void)loadLevel:(int)requestedAction
{
    NSString* levelFileName = [self getLevelName:requestedAction];
    
    bool success = false;
    
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    NSString *filePath = [documentsDirectory stringByAppendingPathComponent:levelFileName];
    
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
        
        GameScreenLevelEditor::getInstance()->load(contentCString, gameScreen);
    }
    
    [self.view makeToast:success ? @"Level loaded successfully" : @"Error occurred while loading level..."];
}

- (void)markLevelAsCompleted:(int)requestedAction
{
    int world = [self calcWorld:requestedAction];
    int level = [self calcLevel:requestedAction];
    int goldenCarrotsFlag = [self calcGoldenCarrotsFlag:requestedAction];
    
    [SaveData setLevelComplete:world level:level goldenCarrotsFlag:goldenCarrotsFlag];
    
    [self sendLevelStats];
}

- (void)sendLevelStats
{
    NSString* userSaveData = @"{";
    for (int i = 1; i <= 5; i++)
    {
        userSaveData = [userSaveData stringByAppendingFormat:@"\"world_%i\":[", i];
        for (int j = 1; j <= 21; j++)
        {
            int levelStats = [SaveData getLevelStats:i level:j];
            
            userSaveData = [userSaveData stringByAppendingFormat:@"%i", levelStats];
            if (j < 21)
            {
                userSaveData = [userSaveData stringByAppendingString:@","];
            }
        }
        userSaveData = [userSaveData stringByAppendingString:@"]"];
        if (i < 5)
        {
            userSaveData = [userSaveData stringByAppendingString:@","];
        }
    }
    userSaveData = [userSaveData stringByAppendingString:@"}"];
    
    const char* userSaveDataCString = [userSaveData cStringUsingEncoding:NSUTF8StringEncoding];
    
    WorldMap::getInstance()->loadUserSaveData(userSaveDataCString);
}

- (void)showMessage:(int)requestedAction
{
    int messageKey = 0;
    
    while (requestedAction >= 1000)
    {
        requestedAction -= 1000;
    }
    
    messageKey = requestedAction;

    NSString* toast = nil;
    
    switch (messageKey) {
        case MESSAGE_NO_END_SIGN_KEY:
            toast = [[NSString alloc] initWithCString:MESSAGE_NO_END_SIGN_VALUE encoding:NSUTF8StringEncoding];
            break;
        case MESSAGE_NO_JON_KEY:
            toast = [[NSString alloc] initWithCString:MESSAGE_NO_JON_VALUE encoding:NSUTF8StringEncoding];
            break;
        case MESSAGE_INVALID_JON_KEY:
            toast = [[NSString alloc] initWithCString:MESSAGE_INVALID_JON_VALUE encoding:NSUTF8StringEncoding];
            break;
        case MESSAGE_NO_COUNT_HISS_KEY:
            toast = [[NSString alloc] initWithCString:MESSAGE_NO_COUNT_HISS_VALUE encoding:NSUTF8StringEncoding];
            break;
        case MESSAGE_INVALID_COUNT_HISS_KEY:
            toast = [[NSString alloc] initWithCString:MESSAGE_INVALID_COUNT_HISS_VALUE encoding:NSUTF8StringEncoding];
            break;
        case MESSAGE_OFFSET_NEEDS_MARKERS_KEY:
            toast = [[NSString alloc] initWithCString:MESSAGE_OFFSET_NEEDS_MARKERS_VALUE encoding:NSUTF8StringEncoding];
            break;
        default:
            break;
    }
    
    if (toast)
    {
        [self.view makeToast:toast];
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
    
    while (requestedAction >= 100000)
    {
        requestedAction -= 100000;
    }
    
    while (requestedAction >= 10000)
    {
        requestedAction -= 10000;
        world++;
    }
    
    return world;
}

- (int)calcLevel:(int)requestedAction
{
    int level = 0;
    
    while (requestedAction >= 100000)
    {
        requestedAction -= 100000;
    }
    
    while (requestedAction >= 10000)
    {
        requestedAction -= 10000;
    }
    
    while (requestedAction >= 100)
    {
        requestedAction -= 100;
        level++;
    }
    
    return level;
}

- (int)calcGoldenCarrotsFlag:(int)requestedAction
{
    while (requestedAction >= 100000)
    {
        requestedAction -= 100000;
    }
    
    while (requestedAction >= 10000)
    {
        requestedAction -= 10000;
    }
    
    while (requestedAction >= 100)
    {
        requestedAction -= 100;
    }
    
    return requestedAction;
}

- (void)onResume
{
    gameScreen->onResume();
}

- (void)onPause
{
    gameScreen->onPause();
    
    [self.soundMgr pauseBackgroundMusic];
}

- (void)initSoundEngine
{
    self.soundMgr = [[CMOpenALSoundManager alloc] init];
    self.soundMgr.soundEffectsVolume = 1;
    self.soundMgr.soundFileNames = [NSArray arrayWithObjects:@"collect_carrot.wav", @"collect_golden_carrot.wav", @"death.wav", @"footstep_left_grass.wav", @"footstep_right_grass.wav", @"footstep_left_cave.wav", @"footstep_right_cave.wav", @"jump_spring.wav", @"landing_grass.wav", @"landing_cave.wav", @"destroy_rock.wav", @"snake_death.wav", @"trigger_transform.wav", @"cancel_transform.wav", @"complete_transform.wav", @"jump_spring_heavy.wav", @"jon_rabbit_jump.wav", @"jon_vampire_jump.wav", @"jon_rabbit_double_jump.wav", @"jon_vampire_double_jump.wav", @"vampire_glide_loop.wav", @"mushroom_bounce.wav", @"jon_burrow_rocksfall.wav", @"sparrow_fly_loop.wav", @"sparrow_die.wav", @"toad_die.wav", @"toad_eat.wav", @"saw_grind_loop.wav", @"fox_bounced_on.wav", @"fox_strike.wav", @"fox_death.wav", @"world_1_bgm_intro.wav", @"mid_boss_bgm_intro.wav", @"mid_boss_owl_swoop.wav", @"mid_boss_owl_tree_smash.wav", @"mid_boss_owl_death.wav", @"screen_transition.wav", nil];
}

@end
