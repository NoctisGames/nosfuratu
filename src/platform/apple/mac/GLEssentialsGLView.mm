/*
 Copyright (C) 2015 Apple Inc. All Rights Reserved.
 See LICENSE.txt for this sample’s licensing information
 
 Abstract:
 OpenGL view subclass.
 */

#import "GLEssentialsGLView.h"

#import "ScreenController.h"

// C++
#include "ScreenInputManager.h"
#include "KeyboardInputManager.h"
#include "MainAssets.h"

#import <OpenGL/OpenGL.h>
#import <OpenGL/gl.h>

#define SUPPORT_RETINA_RESOLUTION 1

@interface GLEssentialsGLView ()
{
    MainScreen *_screen;
    ScreenController *_screenController;
    
    double m_fLastTime;
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
    CVReturn result = [(__bridge GLEssentialsGLView *)displayLinkContext getFrameForTime:outputTime];
    return result;
}

- (void)awakeFromNib
{
    m_fLastTime = CFAbsoluteTimeGetCurrent();
    
    NSWindow *mainWindow = [[[NSApplication sharedApplication] windows] objectAtIndex:0];
    
    [mainWindow toggleFullScreen:self];
    
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
    // TODO
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    MAIN_ASSETS->setUsingDesktopTextureSet(true);
    MAIN_ASSETS->setUsingCompressedTextureSet(false);
    
    _screen = new MainScreen();
    
    _screen->createDeviceDependentResources();
    
    _screenController = [[ScreenController alloc] initWithScreen:_screen getLevelFilePath:^NSString *(NSString *levelFileName)
    {
        NSString *filePath = [NSHomeDirectory() stringByAppendingPathComponent:@"Dropbox/Documents/freelance/NoctisGames/github/nosfuratu-levels"];
        filePath = [filePath stringByAppendingPathComponent:levelFileName];
        return filePath;
    } displayMessageBlock:^(NSString *message)
    {
        NSAlert* alert = [NSAlert alertWithMessageText:message
                                         defaultButton:nil
                                       alternateButton:nil
                                           otherButton:nil
                             informativeTextWithFormat:@""];
        
        [alert runModal];
    } andHandleInterstitialAd:^
    {
        // Empty
    }];
    
    _screen->onResume();
}

- (void)reshape
{
    [super reshape];
    
    // We draw on a secondary thread through the display link. However, when
    // resizing the view, -drawRect is called on the main thread.
    // Add a mutex around to avoid the threads accessing the context
    // simultaneously when resizing.
    CGLLockContext([[self openGLContext] CGLContextObj]);
    
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
    
    // Set the new dimensions in our renderer
    CGFloat width = viewRectPixels.size.width;
    CGFloat height = viewRectPixels.size.height;
    _screen->createWindowSizeDependentResources(width, height, width, height);
    
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
    
    double time = CFAbsoluteTimeGetCurrent();
    
    double deltaTime = time - m_fLastTime;
    m_fLastTime = time;
    
    [_screenController update:deltaTime];
    [_screenController present];
    
    CGLFlushDrawable([[self openGLContext] CGLContextObj]);
    CGLUnlockContext([[self openGLContext] CGLContextObj]);
}

- (void)dealloc
{
    // Stop the display link BEFORE releasing anything in the view
    // otherwise the display link thread may call into the view and crash
    // when it encounters something that has been release
    CVDisplayLinkStop(displayLink);
    
    CVDisplayLinkRelease(displayLink);
    
    delete _screen;
}

- (void)mouseDown:(NSEvent *)theEvent
{
    NSPoint pos = [self convertPoint:[theEvent locationInWindow] fromView:nil];
    SCREEN_INPUT_MANAGER->onTouch(ScreenEventType_DOWN, pos.x, pos.y);
}

- (void)mouseDragged:(NSEvent *)theEvent
{
    NSPoint pos = [self convertPoint:[theEvent locationInWindow] fromView:nil];
    SCREEN_INPUT_MANAGER->onTouch(ScreenEventType_DRAGGED, pos.x, pos.y);
}

- (void)mouseUp:(NSEvent *)theEvent
{
    NSPoint pos = [self convertPoint:[theEvent locationInWindow] fromView:nil];
    SCREEN_INPUT_MANAGER->onTouch(ScreenEventType_UP, pos.x, pos.y);
}

- (void)keyDown:(NSEvent *)event
{
    [self handleKeyEvent:event isUp:false];
}

- (void)keyUp:(NSEvent *)event
{
    [self handleKeyEvent:event isUp:true];
}

- (void)handleKeyEvent:(NSEvent *)event isUp:(bool)isUp
{
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
            
            if (keyChar == NSRightArrowFunctionKey)
            {
                KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_ARROW_KEY_RIGHT, isUp);
                
                [[self window] invalidateCursorRectsForView:self];
                
                return;
            }
            
            if (keyChar == NSUpArrowFunctionKey)
            {
                KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_ARROW_KEY_UP, isUp);
                
                [[self window] invalidateCursorRectsForView:self];
                
                return;
            }
            
            if (keyChar == NSLeftArrowFunctionKey)
            {
                KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_ARROW_KEY_LEFT, isUp);
                
                [[self window] invalidateCursorRectsForView:self];
                
                return;
            }
            
            if (keyChar == NSDownArrowFunctionKey)
            {
                KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_ARROW_KEY_DOWN, isUp);
                
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
                case 'W':
                case 'w':
                    KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_W, isUp);
                    return;
                case 'A':
                case 'a':
                    KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_A, isUp);
                    return;
                case 'S':
                case 's':
                    KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_S, isUp);
                    return;
                case 'D':
                case 'd':
                    KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_D, isUp);
                    return;
                case NSEnterCharacter:
                case NSCarriageReturnCharacter:
                    KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_ENTER, isUp);
                    return;
                case 32:
                    KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_SPACE, isUp);
                    return;
                case NSBackspaceCharacter:
                case NSDeleteCharacter:
                    KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_BACK, isUp);
                    return;
                default:
                    break;
            }
        }
    }
}

@end
