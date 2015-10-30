//
//  GameViewController.m
//  nosfuratu
//
//  Created by Stephen Gowen on 9/5/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#import "GameViewController.h"
#import "GGDDeviceUtil.h"
#import "Music.h"
#import "Sound.h"
#import "UIView+Toast.h"

// C++
#include "IOSOpenGLESGameScreen.h"
#include "IOS8OpenGLESGameScreen.h"
#include "GameConstants.h"
#include "GameScreenStates.h"

@interface GameViewController ()
{
    IOSOpenGLESGameScreen *gameScreen;
}

@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) Music *bgm;
@property (strong, nonatomic) Sound *collectCarrotSound;
@property (strong, nonatomic) Sound *collectGoldenCarrotSound;

@end

@implementation GameViewController

static bool isRunningiOS8 = false;

+ (void)initialize
{
    isRunningiOS8 = SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"8.0");
}

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
    
    CGSize newSize = CGSizeMake(screenSize.width, screenSize.height);
    newSize.width = roundf(newSize.width);
    newSize.height = roundf(newSize.height);
    
    NSLog(@"dimension %f x %f", newSize.width, newSize.height);
    
    [view bindDrawable];
    
    if(isRunningiOS8)
    {
        NSLog(@"Instantiating IOS8OpenGLESGameScreen");
        gameScreen = new IOS8OpenGLESGameScreen(MAX(newSize.width, newSize.height), MIN(newSize.width, newSize.height), [UIScreen mainScreen].applicationFrame.size.width, [UIScreen mainScreen].applicationFrame.size.height);
    }
    else
    {
        NSLog(@"Instantiating IOSOpenGLESGameScreen");
        gameScreen = new IOSOpenGLESGameScreen(MAX(newSize.width, newSize.height), MIN(newSize.width, newSize.height), [UIScreen mainScreen].applicationFrame.size.width, [UIScreen mainScreen].applicationFrame.size.height);
    }
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(onPause)
                                                 name:UIApplicationWillResignActiveNotification
                                               object:nil];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(onResume)
                                                 name:UIApplicationDidBecomeActiveNotification
                                               object:nil];
    
    self.collectCarrotSound = [[Sound alloc] initWithSoundNamed:@"collect_carrot.caf" fromBundle:[NSBundle mainBundle] andMaxNumOfSimultaneousPlays:3];
    self.collectGoldenCarrotSound = [[Sound alloc] initWithSoundNamed:@"collect_golden_carrot.caf" fromBundle:[NSBundle mainBundle] andMaxNumOfSimultaneousPlays:1];
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
            case SOUND_COLLECT_CARROT:
                [self.collectCarrotSound play];
                break;
            case SOUND_COLLECT_GOLDEN_CARROT:
                [self.collectGoldenCarrotSound play];
                break;
            default:
                continue;
        }
    }
}

- (void)handleMusic
{
    short musicId = gameScreen->getCurrentMusicId();
    switch (musicId)
    {
        case MUSIC_STOP:
            if (self.bgm)
            {
                [self.bgm stop];
                self.bgm = nil;
            }
            break;
        case MUSIC_PLAY_DEMO:
            if (!self.bgm)
            {
                self.bgm = [[Music alloc] initWithMusicNamed:@"bgm" fromBundle:[NSBundle mainBundle]];
                [self.bgm setLooping:true];
                [self.bgm setVolume:0.5f];
            }
            
            [self.bgm play];
            break;
        default:
            break;
    }
}

- (void)saveLevel:(int)requestedAction
{
    NSString* levelFileName = [self getLevelName:requestedAction];
    
    bool result = false;
    const char *level_json = LevelEditor::getInstance()->save();
    
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
        
        LevelEditor::getInstance()->load(contentCString);
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
        return [NSString stringWithFormat:@"nosfuratu_w%i_l%i.json", world, level];
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
    
    if (self.bgm)
    {
        [self.bgm pause];
    }
}

@end
