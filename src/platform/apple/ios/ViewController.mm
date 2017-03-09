//
//  ViewController.m
//  nosfuratu
//
//  Created by Stephen Gowen on 9/5/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#import "ViewController.h"

#import "GoogleMobileAds/GoogleMobileAds.h"

// C++
#include "MainScreen.h"
#include "ScreenInputManager.h"
#include "MainAssets.h"
#include "GameConstants.h"

@interface ViewController () <GADInterstitialDelegate>
{
    MainScreen *_screen;
}

@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) GADInterstitial *interstitial;


@end

@implementation ViewController

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
    
    CGRect screenBounds = [[UIScreen mainScreen] nativeBounds];
    
    CGSize size = CGSizeMake(screenBounds.size.width, screenBounds.size.height);
    size.width = roundf(size.width);
    size.height = roundf(size.height);
    
    NSLog(@"dimension %f x %f", size.width, size.height);
    
    [view bindDrawable];
    
    unsigned long long ramSize = [NSProcessInfo processInfo].physicalMemory;
    bool isLowMemoryDevice = ramSize < 1610612736; // 1536 MB
    
    NSLog(@"ramSize: %llu", ramSize);
    NSLog(@"isLowMemoryDevice: %@", isLowMemoryDevice ? @"YES" : @"NO");
    
    MAIN_ASSETS->setUsingCompressedTextureSet(isLowMemoryDevice);
    
    _screen = new MainScreen();
    _screen->createDeviceDependentResources();
    _screen->createWindowSizeDependentResources(MAX(size.width, size.height), MIN(size.width, size.height), [UIScreen mainScreen].applicationFrame.size.width, [UIScreen mainScreen].applicationFrame.size.height);
    
    [self createAndLoadInterstitial];
    
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
    
    _screen->onResume();
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
    SCREEN_INPUT_MANAGER->onTouch(ScreenEventType_DOWN, pos.x, pos.y);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    CGPoint pos = [touch locationInView: [UIApplication sharedApplication].keyWindow];
    SCREEN_INPUT_MANAGER->onTouch(ScreenEventType_DRAGGED, pos.x, pos.y);
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    CGPoint pos = [touch locationInView: [UIApplication sharedApplication].keyWindow];
    SCREEN_INPUT_MANAGER->onTouch(ScreenEventType_UP, pos.x, pos.y);
}

#pragma mark <GLKViewControllerDelegate>

- (void)update
{
    int requestedAction = _screen->getRequestedAction();
    
    switch (requestedAction)
    {
        case REQUESTED_ACTION_DISPLAY_INTERSTITIAL_AD:
            if (self.interstitial.isReady)
            {
                [self.interstitial presentFromRootViewController:self];
            }
            _screen->clearRequestedAction();
            break;
        case REQUESTED_ACTION_UPDATE:
        default:
            break;
    }
    
    _screen->update(self.timeSinceLastUpdate);
}

#pragma mark <GLKViewDelegate>

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    _screen->render();
}

#pragma mark Private

- (void)onResume
{
    _screen->onResume();
}

- (void)onPause
{
    _screen->onPause();
}

- (void)createAndLoadInterstitial
{
#if DEBUG
    self.interstitial = [[GADInterstitial alloc] initWithAdUnitID:@"ca-app-pub-3940256099942544/4411468910"];
#else
    self.interstitial = [[GADInterstitial alloc] initWithAdUnitID:@"ca-app-pub-6017554042572989/7161041758"];
#endif
    
    self.interstitial.delegate = self;
    [self.interstitial loadRequest:[GADRequest request]];
}

- (void)interstitialDidDismissScreen:(GADInterstitial *)interstitial
{
    [self createAndLoadInterstitial];
}

@end
