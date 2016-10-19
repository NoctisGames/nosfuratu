/*
 Copyright (C) 2015 Apple Inc. All Rights Reserved.
 See LICENSE.txt for this sample’s licensing information
 
 Abstract:
 OpenGL view subclass.
 */

#import "GLEssentialsGLView.h"

#import "GameScreenController.h"

// C++
#include "MacOpenGLGameScreen.h"

#import <OpenGL/OpenGL.h>
#import <OpenGL/gl.h>

#define SUPPORT_RETINA_RESOLUTION 0

@interface GLEssentialsGLView ()
{
    MacOpenGLGameScreen *_gameScreen;
    GameScreenController *_gameScreenController;
    
    float _deltaTime;
    double _previousOutputVideoTime;
    
    bool _updateCameFromRefreshTimer;
    bool _updateWasAutomatic;
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
        if (_updateCameFromRefreshTimer)
        {
            _updateCameFromRefreshTimer = false;
        }
        else
        {
            _updateWasAutomatic = true;
        }
        
        if (_updateWasAutomatic)
        {
            [self deinitRefreshTimer];
        }
        
        _deltaTime = (outputTime->videoTime - _previousOutputVideoTime) / (double)outputTime->videoTimeScale;
        _previousOutputVideoTime = outputTime->videoTime;
        
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
    NSWindow *mainWindow = [[[NSApplication sharedApplication] windows] objectAtIndex:0];
    
    [mainWindow setDelegate:self];
    
    NSOpenGLPixelFormatAttribute attrs[] =
	{
        NSOpenGLPFANoRecovery,
        NSOpenGLPFAAccelerated,
		NSOpenGLPFADoubleBuffer,
        NSOpenGLPFAColorSize, 24,
        NSOpenGLPFAAlphaSize, 8,
        NSOpenGLPFADepthSize, 16,
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
	
    _gameScreen->cleanUp();
    
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
    
    _gameScreenController = [[GameScreenController alloc] initWithGameScreen:_gameScreen getLevelFilePath:^NSString *(NSString *levelFileName)
    {
        NSString *filePath = [NSHomeDirectory() stringByAppendingPathComponent:@"Dropbox/Documents/freelance/NoctisGames/github/nosfuratu-levels"];
        filePath = [filePath stringByAppendingPathComponent:levelFileName];
        return filePath;
    } andDisplayMessageBlock:^(NSString *message)
    {
        NSAlert* alert = [NSAlert alertWithMessageText:message
                                         defaultButton:nil
                                       alternateButton:nil
                                           otherButton:nil
                             informativeTextWithFormat:@""];
        
        [alert runModal];
    }];
    
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
    
    [_gameScreenController pause];
    
    // Set the new dimensions in our renderer
	_gameScreen->onResize(viewRectPixels.size.width, viewRectPixels.size.height);
    
    [_gameScreenController resume];
	
	CGLUnlockContext([[self openGLContext] CGLContextObj]);
    
    [self initRefreshTimer];
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

    [_gameScreenController update:_deltaTime];
    [_gameScreenController present];

	CGLFlushDrawable([[self openGLContext] CGLContextObj]);
	CGLUnlockContext([[self openGLContext] CGLContextObj]);
}

- (void)mouseDown:(NSEvent *)theEvent
{
    if (!_gameScreen)
    {
        return;
    }
    
    NSPoint pos = [self convertPoint:[theEvent locationInWindow] fromView:nil];
    _gameScreen->onTouch(DOWN, pos.x, pos.y);
}

- (void)mouseDragged:(NSEvent *)theEvent
{
    if (!_gameScreen)
    {
        return;
    }
    
    NSPoint pos = [self convertPoint:[theEvent locationInWindow] fromView:nil];
    _gameScreen->onTouch(DRAGGED, pos.x, pos.y);
}

- (void)mouseUp:(NSEvent *)theEvent
{
    if (!_gameScreen)
    {
        return;
    }
    
    NSPoint pos = [self convertPoint:[theEvent locationInWindow] fromView:nil];
    _gameScreen->onTouch(UP, pos.x, pos.y);
}

- (void)keyDown:(NSEvent *)event
{
    if (!_gameScreen)
    {
        return;
    }
    
    if ([event modifierFlags] & NSNumericPadKeyMask)
    {
        // arrow keys have this mask
        NSString *theArrow = [event charactersIgnoringModifiers];
        
        unichar keyChar = 0;
        
        if ([theArrow length] == 0)
        {
            return; // reject dead keys
        }
        
        if ([theArrow length] == 1)
        {
            keyChar = [theArrow characterAtIndex:0];
            
            if (keyChar == NSLeftArrowFunctionKey)
            {
                _gameScreen->onTouch(DOWN, 500, 500);
                _gameScreen->onTouch(DRAGGED, 500, 500);
                _gameScreen->onTouch(DRAGGED, 0, 500);
                _gameScreen->onTouch(UP, 0, 500);
                
                [[self window] invalidateCursorRectsForView:self];
                
                return;
            }
            
            if (keyChar == NSUpArrowFunctionKey)
            {
                _gameScreen->onTouch(DOWN, 500, 500);
                _gameScreen->onTouch(DRAGGED, 500, 500);
                _gameScreen->onTouch(DRAGGED, 500, 900);
                _gameScreen->onTouch(UP, 500, 900);
                
                [[self window] invalidateCursorRectsForView:self];
                
                return;
            }
            
            if (keyChar == NSRightArrowFunctionKey)
            {
                _gameScreen->onTouch(DOWN, 500, 500);
                _gameScreen->onTouch(DRAGGED, 500, 500);
                _gameScreen->onTouch(DRAGGED, 900, 500);
                _gameScreen->onTouch(UP, 900, 500);
                
                [[self window] invalidateCursorRectsForView:self];
                
                return;
            }
            
            if (keyChar == NSDownArrowFunctionKey)
            {
                _gameScreen->onTouch(DOWN, 500, 500);
                _gameScreen->onTouch(DRAGGED, 500, 500);
                _gameScreen->onTouch(DRAGGED, 500, 0);
                _gameScreen->onTouch(UP, 500, 0);
                
                [[self window] invalidateCursorRectsForView:self];
                
                return;
            }
        }
    }
    else
    {
        NSString *characters = [event characters];
        
        if ([characters length] == 0)
        {
            return; // reject dead keys
        }
        
        unichar keyChar = 0;
        
        if ([characters length] == 1)
        {
            keyChar = [characters characterAtIndex:0];
            
            switch (keyChar)
            {
                case 'w':
                {
                    _gameScreen->onTouch(DOWN, 300, 300);
                    _gameScreen->onTouch(UP, 300, 300);
                }
                    return;
                case 's':
                {
                    _gameScreen->onTouch(DOWN, 300, 300);
                }
                    return;
                default:
                    break;
            }
        }
    }
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

static NSTimer *renderTimer = nil;

- (void)windowDidResignMain:(NSNotification *)notification
{
    [self deinitRefreshTimer];
    
    [_gameScreenController pause];
    
    [self setNeedsDisplay:YES];
}

- (void)windowDidBecomeMain:(NSNotification *)notification
{
    [self initRefreshTimer];
    
    [_gameScreenController resume];
    
    [self setNeedsDisplay:YES];
}

- (void)initRefreshTimer
{
    [self deinitRefreshTimer];
    
    renderTimer = [NSTimer timerWithTimeInterval:0.01666666666667 target:self selector:@selector(timerFired:) userInfo:nil repeats:YES];
    
    [[NSRunLoop currentRunLoop] addTimer:renderTimer
                                 forMode:NSDefaultRunLoopMode];
    
    [[NSRunLoop currentRunLoop] addTimer:renderTimer
                                 forMode:NSEventTrackingRunLoopMode]; // Ensure timer fires during resize
}

- (void)deinitRefreshTimer
{
    if (renderTimer)
    {
        [renderTimer invalidate];
    }
}

// Timer callback method

- (void)timerFired:(id)sender
{
    // It is good practice in a Cocoa application to allow the system to send the -drawRect:
    // message when it needs to draw, and not to invoke it directly from the timer.
    // All we do here is tell the display it needs a refresh
    
    _updateCameFromRefreshTimer = true;
    [self setNeedsDisplay:YES];
}

@end
