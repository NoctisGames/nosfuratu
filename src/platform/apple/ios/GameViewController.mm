//
//  GameViewController.m
//  nosfuratu
//
//  Created by Stephen Gowen on 9/5/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#import "GameViewController.h"

#import "UIView+Toast.h"
#import "GameScreenController.h"

// C++
#include "IOSOpenGLESGameScreen.h"

@interface GameViewController ()
{
    IOSOpenGLESGameScreen *gameScreen;
    GameScreenController *_gameScreenController;
}

@property (strong, nonatomic) EAGLContext *context;

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
    
    unsigned long long ramSize = [NSProcessInfo processInfo].physicalMemory;
    bool isLowMemoryDevice = ramSize < 1610612736; // 1536 MB
    
    NSLog(@"ramSize: %llu", ramSize);
    NSLog(@"isLowMemoryDevice: %@", isLowMemoryDevice ? @"YES" : @"NO");
    
    gameScreen = new IOSOpenGLESGameScreen(MAX(size.width, size.height), MIN(size.width, size.height), [UIScreen mainScreen].applicationFrame.size.width, [UIScreen mainScreen].applicationFrame.size.height, isLowMemoryDevice);
    
    _gameScreenController = [[GameScreenController alloc] initWithGameScreen:gameScreen getLevelFilePath:^NSString *(NSString *levelFileName)
    {
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *documentsDirectory = [paths objectAtIndex:0];
        NSString *filePath = [documentsDirectory stringByAppendingPathComponent:levelFileName];
        
        return filePath;
    } andDisplayMessageBlock:^(NSString *message)
    {
        [self.view makeToast:message];
    }];
    
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
    [_gameScreenController update:self.timeSinceLastUpdate];
}

#pragma mark <GLKViewDelegate>

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    [_gameScreenController present];
}

#pragma mark Private

- (void)onResume
{
    [_gameScreenController resume];
}

- (void)onPause
{
    [_gameScreenController pause];
}

@end
