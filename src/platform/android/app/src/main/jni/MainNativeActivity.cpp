/*
 * Copyright 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//--------------------------------------------------------------------------------
// Include files
//--------------------------------------------------------------------------------
#include <jni.h>
#include <errno.h>

#include <vector>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/native_window_jni.h>
#include <cpu-features.h>

#include "NDKHelper.h"

#include "macros.h"
#include "MainScreen.h"
#include "MainScreenTitle.h"
#include "ScreenInputManager.h"
#include "KeyboardInputManager.h"
#include "AndroidAudioEngineHelper.h"
#include "AndroidAssetDataHandler.h"
#include "MainAssets.h"
#include "OpenGLManager.h"
#include "GameConstants.h"
#include "Vector2D.h"

struct android_app;

class Engine
{
    ndk_helper::GLContext* gl_context_;
    
    bool initialized_resources_;
    bool has_focus_;
    float m_fLastTime;
    
    android_app* app_;
    
    ASensorManager* sensor_manager_;
    const ASensor* accelerometer_sensor_;
    ASensorEventQueue* sensor_event_queue_;
    
    void DisplayInterstitialAdIfLoaded();
    
public:
    static void HandleCmd( struct android_app* app,
                          int32_t cmd );
    static int32_t HandleInput( android_app* app,
                               AInputEvent* event );
    
    Engine();
    ~Engine();
    void SetState( android_app* state );
    int InitDisplay();
    void LoadResources();
    void UnloadResources();
    void DrawFrame();
    void InitializeInterstitialAds();
    void TermDisplay();
    void TrimMemory();
    bool IsReady();
    
    void InitSensors();
    void ProcessSensors( int32_t id );
    void SuspendSensors();
    void ResumeSensors();
    
private:
    MainScreen* m_screen;
};

//-------------------------------------------------------------------------
//Ctor
//-------------------------------------------------------------------------
Engine::Engine() :
initialized_resources_( false ),
has_focus_( false ),
app_( NULL ),
sensor_manager_( NULL ),
accelerometer_sensor_( NULL ),
sensor_event_queue_( NULL ),
m_fLastTime(0)
{
    gl_context_ = ndk_helper::GLContext::GetInstance();
}

//-------------------------------------------------------------------------
//Dtor
//-------------------------------------------------------------------------
Engine::~Engine()
{
}

/**
 * Load resources
 */
void Engine::LoadResources()
{
    JNIEnv *jni;
    app_->activity->vm->AttachCurrentThread(&jni, NULL);
    
    ANDROID_AUDIO_ENGINE_HELPER->init(jni, app_->activity->clazz);
    
    ANDROID_ASSETS->init(jni, app_->activity->clazz);
    
    MAIN_ASSETS->setUsingCompressedTextureSet(gl_context_->GetScreenWidth() < 2560);
    
    m_screen = new MainScreen();
    
    int width = gl_context_->GetScreenWidth();
    int height = gl_context_->GetScreenHeight();
    
    m_screen->createDeviceDependentResources();
    
    OGLManager->setScreenSize(width, height);
    
    m_screen->createWindowSizeDependentResources(width > 1440 ? 1440 : width, height > 900 ? 900 : height, width, height);
    
    app_->activity->vm->DetachCurrentThread();
    return;
}

/**
 * Unload resources
 */
void Engine::UnloadResources()
{
    ANDROID_AUDIO_ENGINE_HELPER->deinit();

    delete m_screen;
    m_screen = nullptr;
    
    m_screen->releaseDeviceDependentResources();
}

/**
 * Initialize an EGL context for the current display.
 */
int Engine::InitDisplay()
{
    if (!initialized_resources_)
    {
        gl_context_->Init(app_->window);
        
        LoadResources();
        initialized_resources_ = true;
    }
    else
    {
        // initialize OpenGL ES and EGL
        if (EGL_SUCCESS != gl_context_->Resume(app_->window))
        {
            UnloadResources();
            LoadResources();
        }
    }
    
    return 0;
}

/**
 * Just the current frame in the display.
 */
void Engine::DrawFrame()
{
    int requestedAction = m_screen->getRequestedAction();
    
    switch (requestedAction)
    {
        case REQUESTED_ACTION_DISPLAY_INTERSTITIAL_AD:
            DisplayInterstitialAdIfLoaded();
            m_screen->clearRequestedAction();
            break;
        case REQUESTED_ACTION_UPDATE:
        default:
            break;
    }
    
    // TODO, do not hardcode delta time
    m_screen->update(0.016666666666667f);
    
    m_screen->render();
    
    // Swap
    if (EGL_SUCCESS != gl_context_->Swap())
    {
        UnloadResources();
        LoadResources();
    }
}

/**
 * Tear down the EGL context currently associated with the display.
 */
void Engine::TermDisplay()
{
    gl_context_->Suspend();
}

void Engine::TrimMemory()
{
    LOGI("Trimming memory");
    
    gl_context_->Invalidate();
}
/**
 * Process the next input event.
 */
int32_t Engine::HandleInput(android_app* app, AInputEvent* event)
{
    Engine* eng = (Engine*) app->userData;
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
    {
        int32_t action = AMotionEvent_getAction(event);
        int32_t pointerIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
        uint32_t flags = action & AMOTION_EVENT_ACTION_MASK;
        
        int32_t pointerCount = AMotionEvent_getPointerCount(event);
        
        switch (action)
        {
            case AMOTION_EVENT_ACTION_DOWN:
            case AMOTION_EVENT_ACTION_POINTER_DOWN:
            {
                float x = AMotionEvent_getX(event, pointerIndex);
                float y = AMotionEvent_getY(event, pointerIndex);
                SCREEN_INPUT_MANAGER->onTouch(ScreenEventType_DOWN, x, y);
            }
                break;
            case AMOTION_EVENT_ACTION_UP:
            case AMOTION_EVENT_ACTION_POINTER_UP:
            case AMOTION_EVENT_ACTION_CANCEL:
            {
                float x = AMotionEvent_getX(event, pointerIndex);
                float y = AMotionEvent_getY(event, pointerIndex);
                SCREEN_INPUT_MANAGER->onTouch(ScreenEventType_UP, x, y);
            }
                break;
            case AMOTION_EVENT_ACTION_MOVE:
            {
                for (int i = 0; i < pointerCount; ++i)
                {
                    pointerIndex = i;
                    float x = AMotionEvent_getX(event, pointerIndex);
                    float y = AMotionEvent_getY(event, pointerIndex);
                    SCREEN_INPUT_MANAGER->onTouch(ScreenEventType_DRAGGED, x, y);
                }
            }
                break;
        }
        
        return 1;
    }
    
    return 0;
}

/**
 * Process the next main command.
 */
void Engine::HandleCmd(struct android_app* app, int32_t cmd)
{
    Engine* eng = (Engine*) app->userData;
    switch (cmd)
    {
        case APP_CMD_SAVE_STATE:
            break;
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            if( app->window != NULL )
            {
                eng->InitDisplay();
                eng->DrawFrame();
                eng->InitializeInterstitialAds();
            }
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
            eng->TermDisplay();
            eng->has_focus_ = false;
            break;
        case APP_CMD_STOP:
            break;
        case APP_CMD_GAINED_FOCUS:
            eng->ResumeSensors();
            //Start animation
            eng->has_focus_ = true;
            break;
        case APP_CMD_LOST_FOCUS:
            eng->SuspendSensors();
            // Also stop animating.
            eng->has_focus_ = false;
            eng->DrawFrame();
            break;
        case APP_CMD_LOW_MEMORY:
            //Free up GL resources
            eng->TrimMemory();
            break;
    }
}

//-------------------------------------------------------------------------
//Sensor handlers
//-------------------------------------------------------------------------
void Engine::InitSensors()
{
    sensor_manager_ = ASensorManager_getInstance();
    accelerometer_sensor_ = ASensorManager_getDefaultSensor(sensor_manager_, ASENSOR_TYPE_ACCELEROMETER);
    sensor_event_queue_ = ASensorManager_createEventQueue(sensor_manager_, app_->looper, LOOPER_ID_USER, NULL, NULL);
}

void Engine::ProcessSensors(int32_t id)
{
    // If a sensor has data, process it now.
    if( id == LOOPER_ID_USER )
    {
        if( accelerometer_sensor_ != NULL )
        {
            ASensorEvent event;
            while( ASensorEventQueue_getEvents( sensor_event_queue_, &event, 1 ) > 0 )
            {
                // TODO?
            }
        }
    }
}

void Engine::ResumeSensors()
{
    // When our app gains focus, we start monitoring the accelerometer.
    if( accelerometer_sensor_ != NULL )
    {
        ASensorEventQueue_enableSensor( sensor_event_queue_, accelerometer_sensor_ );
        // We'd like to get 60 events per second (in us).
        ASensorEventQueue_setEventRate( sensor_event_queue_, accelerometer_sensor_,
                                       (1000L / 60) * 1000 );
    }
}

void Engine::SuspendSensors()
{
    // When our app loses focus, we stop monitoring the accelerometer.
    // This is to avoid consuming battery while not being used.
    if( accelerometer_sensor_ != NULL )
    {
        ASensorEventQueue_disableSensor( sensor_event_queue_, accelerometer_sensor_ );
    }
}

//-------------------------------------------------------------------------
//Misc
//-------------------------------------------------------------------------
void Engine::SetState( android_app* state )
{
    app_ = state;
}

bool Engine::IsReady()
{
    if (has_focus_)
    {
        return true;
    }
    
    return false;
}

void Engine::InitializeInterstitialAds()
{
    JNIEnv *jni;
    app_->activity->vm->AttachCurrentThread( &jni, NULL );
    
    //Default class retrieval
    jclass clazz = jni->GetObjectClass(app_->activity->clazz);
    jmethodID methodID = jni->GetMethodID(clazz, "initializeInterstitialAds", "()V");
    jni->CallVoidMethod(app_->activity->clazz, methodID);
    
    app_->activity->vm->DetachCurrentThread();
    return;
}

void Engine::DisplayInterstitialAdIfLoaded()
{
    JNIEnv *jni;
    app_->activity->vm->AttachCurrentThread( &jni, NULL );
    
    //Default class retrieval
    jclass clazz = jni->GetObjectClass(app_->activity->clazz);
    jmethodID methodID = jni->GetMethodID(clazz, "displayInterstitialAdIfLoaded", "()V");
    jni->CallVoidMethod(app_->activity->clazz, methodID);
    
    app_->activity->vm->DetachCurrentThread();
    return;
}

Engine g_engine;

/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(android_app* state)
{
    app_dummy();
    
    g_engine.SetState(state);
    
    state->userData = &g_engine;
    state->onAppCmd = Engine::HandleCmd;
    state->onInputEvent = Engine::HandleInput;
    
#ifdef USE_NDK_PROFILER
    monstartup("libandroid_main.so");
#endif
    
    // Prepare to monitor accelerometer
    g_engine.InitSensors();
    
    // loop waiting for stuff to do.
    while (1)
    {
        // Read all pending events.
        int id;
        int events;
        android_poll_source* source;
        
        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((id = ALooper_pollAll(g_engine.IsReady() ? 0 : -1, NULL, &events, (void**) &source)) >= 0)
        {
            // Process this event.
            if (source != NULL)
            {
                source->process(state, source);
            }
            
            g_engine.ProcessSensors(id);
            
            // Check if we are exiting.
            if (state->destroyRequested != 0)
            {
                g_engine.TermDisplay();
                return;
            }
        }
        
        if (g_engine.IsReady())
        {
            // Drawing is throttled to the screen update rate, so there
            // is no need to do timing here.
            g_engine.DrawFrame();
        }
    }
}
