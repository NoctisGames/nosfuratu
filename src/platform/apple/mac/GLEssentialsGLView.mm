/*
 Copyright (C) 2015 Apple Inc. All Rights Reserved.
 See LICENSE.txt for this sample’s licensing information
 
 Abstract:
 OpenGL view subclass.
 */

#import "GLEssentialsGLView.h"

#import "SaveData.h"

// Sound Engine
#import "SoundManager.h"

// C++
#include "MacOpenGLGameScreen.h"
#include "GameConstants.h"
#include "GameScreenLevelEditor.h"
#include "GameScreenWorldMap.h"
#include "Game.h"

#import <OpenGL/OpenGL.h>
#import <OpenGL/gl.h>

#define SUPPORT_RETINA_RESOLUTION 1

@interface GLEssentialsGLView ()
{
    MacOpenGLGameScreen *_gameScreen;
    NSArray *_soundFileNames;
    double _deltaTime;
    double _startTime;
    NSString *_lastKnownMusicName;
    BOOL _lastKnownMusicLooping;
}

@end

@implementation GLEssentialsGLView

- (CVReturn)getFrameForTime:(const CVTimeStamp *)outputTime
{
	// There is no autorelease pool when this method is called
	// because it will be called from a background thread.
    // It's important to create one or app can leak objects.
    @autoreleasepool
    {
        _deltaTime = 1.0 / (outputTime->rateScalar * (double)outputTime->videoTimeScale / (double)outputTime->videoRefreshPeriod);
        
        [self drawView];
    }
	
    return kCVReturnSuccess;
}

// This is the renderer output callback function
static CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink,
									  const CVTimeStamp* now,
									  const CVTimeStamp* outputTime,
									  CVOptionFlags flagsIn,
									  CVOptionFlags* flagsOut, 
									  void* displayLinkContext)
{
    CVReturn result = [(__bridge GLEssentialsGLView*)displayLinkContext getFrameForTime:outputTime];
    return result;
}

- (void)awakeFromNib
{
    [[SoundManager sharedManager] prepareToPlay];
    
    [self initSoundEngine];
    
    NSWindow *mainWindow = [[[NSApplication sharedApplication] windows] objectAtIndex:0];
    
    [mainWindow setDelegate:self];
    
    NSOpenGLPixelFormatAttribute attrs[] =
	{
		NSOpenGLPFADoubleBuffer,
		NSOpenGLPFADepthSize, 24,
		// Must specify the 3.2 Core Profile to use OpenGL 3.2
#if ESSENTIAL_GL_PRACTICES_SUPPORT_GL3 
		NSOpenGLPFAOpenGLProfile,
		NSOpenGLProfileVersion3_2Core,
#endif
		0
	};
	
	NSOpenGLPixelFormat *pf = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
	
	if (!pf)
	{
		NSLog(@"No OpenGL pixel format");
	}
	   
    NSOpenGLContext* context = [[NSOpenGLContext alloc] initWithFormat:pf shareContext:nil];
    
#if ESSENTIAL_GL_PRACTICES_SUPPORT_GL3 && defined(DEBUG)
	// When we're using a CoreProfile context, crash if we call a legacy OpenGL function
	// This will make it much more obvious where and when such a function call is made so
	// that we can remove such calls.
	// Without this we'd simply get GL_INVALID_OPERATION error for calling legacy functions
	// but it would be more difficult to see where that function was called.
	CGLEnable([context CGLContextObj], kCGLCECrashOnRemovedFunctions);
#endif
	
    [self setPixelFormat:pf];
    
    [self setOpenGLContext:context];
    
#if SUPPORT_RETINA_RESOLUTION
    // Opt-In to Retina resolution
    [self setWantsBestResolutionOpenGLSurface:YES];
#endif // SUPPORT_RETINA_RESOLUTION
}

- (void)prepareOpenGL
{
	[super prepareOpenGL];
	
	// Make all the OpenGL calls to setup rendering  
	//  and build the necessary rendering objects
	[self initGL];
	
	// Create a display link capable of being used with all active displays
	CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
	
	// Set the renderer output callback function
	CVDisplayLinkSetOutputCallback(displayLink, &MyDisplayLinkCallback, (__bridge void*)self);
	
	// Set the display link for the current renderer
	CGLContextObj cglContext = [[self openGLContext] CGLContextObj];
	CGLPixelFormatObj cglPixelFormat = [[self pixelFormat] CGLPixelFormatObj];
	CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, cglContext, cglPixelFormat);
	
	// Activate the display link
	CVDisplayLinkStart(displayLink);
	
	// Register to be notified when the window closes so we can stop the displaylink
	[[NSNotificationCenter defaultCenter] addObserver:self
											 selector:@selector(windowWillClose:)
												 name:NSWindowWillCloseNotification
											   object:[self window]];
}

- (void)windowWillClose:(NSNotification *)notification
{
	// Stop the display link when the window is closing because default
	// OpenGL render buffers will be destroyed.  If display link continues to
	// fire without renderbuffers, OpenGL draw calls will set errors.
	
	CVDisplayLinkStop(displayLink);
}

- (void)initGL
{
	// The reshape function may have changed the thread to which our OpenGL
	// context is attached before prepareOpenGL and initGL are called.  So call
	// makeCurrentContext to ensure that our OpenGL context current to this 
	// thread (i.e. makeCurrentContext directs all OpenGL calls on this thread
	// to [self openGLContext])
	[[self openGLContext] makeCurrentContext];
	
	// Synchronize buffer swaps with vertical refresh rate
	GLint swapInt = 1;
	[[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
	
	// Init our renderer.  Use 0 for the defaultFBO which is appropriate for
	// OSX (but not iOS since iOS apps must create their own FBO)
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    NSRect viewRectPixels = [self getScreenBounds];
    
    _gameScreen = new MacOpenGLGameScreen(viewRectPixels.size.width, viewRectPixels.size.height);
    
    _gameScreen->onResume();
}

- (void)reshape
{	
	[super reshape];
	
	// We draw on a secondary thread through the display link. However, when
	// resizing the view, -drawRect is called on the main thread.
	// Add a mutex around to avoid the threads accessing the context
	// simultaneously when resizing.
	CGLLockContext([[self openGLContext] CGLContextObj]);

	NSRect viewRectPixels = [self getScreenBounds];
    
    // Set the new dimensions in our renderer
	_gameScreen->onResize(viewRectPixels.size.width, viewRectPixels.size.height);
	
	CGLUnlockContext([[self openGLContext] CGLContextObj]);
}

- (void)renewGState
{	
	// Called whenever graphics state updated (such as window resize)
	
	// OpenGL rendering is not synchronous with other rendering on the OSX.
	// Therefore, call disableScreenUpdatesUntilFlush so the window server
	// doesn't render non-OpenGL content in the window asynchronously from
	// OpenGL content, which could cause flickering.  (non-OpenGL content
	// includes the title bar and drawing done by the app with other APIs)
	[[self window] disableScreenUpdatesUntilFlush];

	[super renewGState];
}

- (void)drawRect:(NSRect)theRect
{
	// Called during resize operations
	
	// Avoid flickering during resize by drawiing	
	[self drawView];
}

- (void)drawView
{	 
	[[self openGLContext] makeCurrentContext];

	// We draw on a secondary thread through the display link
	// When resizing the view, -reshape is called automatically on the main
	// thread. Add a mutex around to avoid the threads accessing the context
	// simultaneously when resizing
	CGLLockContext([[self openGLContext] CGLContextObj]);

    [self updateGame];
    _gameScreen->render();
    
    [self handleSound];
    [self handleMusic];

	CGLFlushDrawable([[self openGLContext] CGLContextObj]);
	CGLUnlockContext([[self openGLContext] CGLContextObj]);
}

- (void)mouseDown:(NSEvent *)theEvent
{
    NSPoint pos = [self convertPoint:[theEvent locationInWindow] fromView:nil];
    _gameScreen->onTouch(DOWN, pos.x, pos.y);
}

- (void)mouseDragged:(NSEvent *)theEvent
{
    NSPoint pos = [self convertPoint:[theEvent locationInWindow] fromView:nil];
    _gameScreen->onTouch(DRAGGED, pos.x, pos.y);
}

- (void)mouseUp:(NSEvent *)theEvent
{
    NSPoint pos = [self convertPoint:[theEvent locationInWindow] fromView:nil];
    _gameScreen->onTouch(UP, pos.x, pos.y);
}

- (void)updateGame
{
    int requestedAction = _gameScreen->getRequestedAction();
    if (requestedAction >= 1000)
    {
        requestedAction /= 1000;
    }
    
    switch (requestedAction)
    {
        case REQUESTED_ACTION_UPDATE:
            _gameScreen->update(_deltaTime);
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

#pragma mark <Private>

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
            [[SoundManager sharedManager] stopMusic:NO];
            break;
        case MUSIC_RESUME:
            [self playMusic:_lastKnownMusicName isLooping:_lastKnownMusicLooping];
            break;
        case MUSIC_SET_VOLUME:
        {
            float volume = rawMusicId / 100.0f / 2.0f; // On iOS, volume starts off at 0.5
            if (volume < 0)
            {
                volume = 0;
            }
            
            [SoundManager sharedManager].musicVolume = volume;
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
    if ([SoundManager sharedManager].playingMusic)
    {
        [[SoundManager sharedManager] stopMusic:NO];
    }
    
    [SoundManager sharedManager].musicVolume = 0.5f;
    [[SoundManager sharedManager] playMusic:fileName looping:isLooping fadeIn:NO];
    
    _lastKnownMusicName = fileName;
    _lastKnownMusicLooping = isLooping;
}

- (void)playSound:(int)soundId isLooping:(bool)isLooping
{
    int soundIndex = soundId - 1;
    NSString *soundName = [_soundFileNames objectAtIndex:soundIndex];
    [[SoundManager sharedManager] playSound:soundName looping:isLooping];
}

- (void)playSound:(int)soundId
{
    int soundIndex = soundId - 1;
    NSString *soundName = [_soundFileNames objectAtIndex:soundIndex];
    [[SoundManager sharedManager] playSound:soundName looping:NO];
}

- (void)stopSound:(int)soundId
{
    int soundIndex = soundId - 1;
    NSString *soundName = [_soundFileNames objectAtIndex:soundIndex];
    [[SoundManager sharedManager] stopSound:soundName];
}

- (void)saveLevel:(int)requestedAction
{
    NSString* levelFileName = [self getLevelName:requestedAction];
    
    bool result = false;
    const char *level_json = GameScreenLevelEditor::getInstance()->save();
    
    if (level_json)
    {
        NSString *json = [[NSString alloc] initWithCString:level_json encoding:NSUTF8StringEncoding];
        NSString *filePath = [NSHomeDirectory() stringByAppendingPathComponent:@"Dropbox/Documents/freelance/NoctisGames/github/nosfuratu-levels"];
        filePath = [filePath stringByAppendingPathComponent:levelFileName];
        
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
    
    NSString *filePath = [NSHomeDirectory() stringByAppendingPathComponent:@"Dropbox/Documents/freelance/NoctisGames/github/nosfuratu-levels"];
    filePath = [filePath stringByAppendingPathComponent:levelFileName];
    
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
        NSAlert* alert = [NSAlert alertWithMessageText:message
                                         defaultButton:nil
                                       alternateButton:nil
                                           otherButton:nil
                             informativeTextWithFormat:@""];
        
        [alert runModal];
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

- (void)dealloc
{
	// Stop the display link BEFORE releasing anything in the view
    // otherwise the display link thread may call into the view and crash
    // when it encounters something that has been release
	CVDisplayLinkStop(displayLink);

	CVDisplayLinkRelease(displayLink);
}

- (NSRect)getScreenBounds
{
    // Get the view size in Points
    NSRect viewRectPoints = [self bounds];
    
#if SUPPORT_RETINA_RESOLUTION
    
    // Rendering at retina resolutions will reduce aliasing, but at the potential
    // cost of framerate and battery life due to the GPU needing to render more
    // pixels.
    
    // Any calculations the renderer does which use pixel dimentions, must be
    // in "retina" space.  [NSView convertRectToBacking] converts point sizes
    // to pixel sizes.  Thus the renderer gets the size in pixels, not points,
    // so that it can set it's viewport and perform and other pixel based
    // calculations appropriately.
    // viewRectPixels will be larger than viewRectPoints for retina displays.
    // viewRectPixels will be the same as viewRectPoints for non-retina displays
    NSRect viewRectPixels = [self convertRectToBacking:viewRectPoints];
    
#else //if !SUPPORT_RETINA_RESOLUTION
    
    // App will typically render faster and use less power rendering at
    // non-retina resolutions since the GPU needs to render less pixels.
    // There is the cost of more aliasing, but it will be no-worse than
    // on a Mac without a retina display.
    
    // Points:Pixels is always 1:1 when not supporting retina resolutions
    NSRect viewRectPixels = viewRectPoints;
    
#endif // !SUPPORT_RETINA_RESOLUTION
    
    NSLog(@"dimension %f x %f", viewRectPixels.size.width, viewRectPixels.size.height);
    
    return viewRectPixels;
}

#pragma mark <NSWindowDelegate>

- (void)windowDidResignMain:(NSNotification *)notification
{
    _gameScreen->onPause();
    
    [[SoundManager sharedManager] stopMusic:NO];
    
    [self setNeedsDisplay:YES];
}

- (void)windowDidBecomeMain:(NSNotification *)notification
{
    [self setNeedsDisplay:YES];
    
    _gameScreen->onResume();
}

// Timer callback method

- (void)timerFired:(id)sender
{
    // It is good practice in a Cocoa application to allow the system to send the -drawRect:
    // message when it needs to draw, and not to invoke it directly from the timer.
    // All we do here is tell the display it needs a refresh
    
    [self setNeedsDisplay:YES];
}

- (void)initSoundEngine
{
    [SoundManager sharedManager].musicVolume = 0.5f;
    [SoundManager sharedManager].soundVolume = 1;
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
}

@end
