//
//  GameViewController.m
//  nosfuratu
//
//  Created by Stephen Gowen on 9/5/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#import "GameViewController.h"
#import "UIView+Toast.h"

// Sound Engine
#import "CMOpenALSoundManager.h"

// C++
#include "IOSOpenGLESGameScreen.h"
#include "GameConstants.h"
#include "GameScreenLevelEditor.h"

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
    if (requestedAction >= 1000)
    {
        requestedAction /= 1000;
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
                [self playSound:soundId isLooping:true];
                break;
            case SOUND_STOP_JON_VAMPIRE_GLIDE:
                [self stopSound:SOUND_JON_VAMPIRE_GLIDE];
                break;
            default:
                [self playSound:soundId];
                break;
        }
    }
}

- (void)handleMusic
{
    short musicId = gameScreen->getCurrentMusicId();
    switch (musicId)
    {
        case MUSIC_STOP:
            [self.soundMgr stopBackgroundMusic];
            break;
        case MUSIC_RESUME:
            [self.soundMgr resumeBackgroundMusic];
            break;
        case MUSIC_PLAY_DEMO:
            // start background music
            [self initSoundEngine];
            self.soundMgr.backgroundMusicVolume = 0.5f;
            self.soundMgr.soundEffectsVolume = 1;
            [self.soundMgr playBackgroundMusic:@"bgm.wav" forcePlay:YES]; // you could use forcePlay: YES if you wanted to stop any other audio source (iPod)
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
        
        GameScreenLevelEditor::getInstance()->load(contentCString);
    }
    
    [self.view makeToast:success ? @"Level loaded successfully" : @"Error occurred while loading level..."];
}

- (NSString *)getLevelName:(int)requestedAction
{
    int world = 0;
    int level = 0;
    while (requestedAction >= 1000)
    {
        requestedAction -= 1000;
    }
    
    while (requestedAction >= 100)
    {
        requestedAction -= 100;
        world++;
    }
    
    while (requestedAction >= 1)
    {
        requestedAction--;
        level++;
    }
    
    if (world > 0 && level > 0)
    {
        return [NSString stringWithFormat:@"nosfuratu_c%i_l%i.json", world, level];
    }
    else
    {
        return @"nosfuratu.json";
    }
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
    self.soundMgr.soundFileNames = [NSArray arrayWithObjects:@"collect_carrot.wav", @"collect_golden_carrot.wav", @"death.wav", @"footstep_left_grass.wav", @"footstep_right_grass.wav", @"footstep_left_cave.wav", @"footstep_right_cave.wav", @"jump_spring.wav", @"landing_grass.wav", @"landing_cave.wav", @"destroy_rock.wav", @"snake_death.wav", @"trigger_transform.wav", @"cancel_transform.wav", @"complete_transform.wav", @"jump_spring_heavy.wav", @"jon_rabbit_jump.wav", @"jon_vampire_jump.wav", @"jon_rabbit_double_jump.wav", @"jon_vampire_double_jump.wav", @"vampire_glide_loop.wav", @"mushroom_bounce.wav", nil];
}

@end
