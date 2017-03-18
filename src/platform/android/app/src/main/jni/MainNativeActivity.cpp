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

//--------------------------------------------------------------------------------
// Constants
//--------------------------------------------------------------------------------
const int32_t DOUBLE_TAP_TIMEOUT = 300 * 1000000;
const int32_t TAP_TIMEOUT = 180 * 1000000;
const int32_t DOUBLE_TAP_SLOP = 100;
const int32_t TOUCH_SLOP = 8;

enum
{
    GESTURE_STATE_NONE = 0,
    GESTURE_STATE_START = 1,
    GESTURE_STATE_MOVE = 2,
    GESTURE_STATE_END = 4,
    GESTURE_STATE_ACTION = (GESTURE_STATE_START | GESTURE_STATE_END),
};
typedef int32_t GESTURE_STATE;

/******************************************************************
 * Base class of Gesture Detectors
 * GestureDetectors handles input events and detect gestures
 * Note that different detectors may detect gestures with an event at
 * same time. The caller needs to manage gesture priority accordingly
 *
 */
class GestureDetector
{
protected:
    float dp_factor_;
public:
    GestureDetector()
    {
        dp_factor_ = 1.f;
    }
    
    virtual ~GestureDetector()
    {
    }
    
    virtual void SetConfiguration(AConfiguration* config)
    {
        dp_factor_ = 160.f / AConfiguration_getDensity( config );
    }
    
    virtual GESTURE_STATE Detect( const AInputEvent* motion_event ) = 0;
};

/******************************************************************
 * Tap gesture detector
 * Returns GESTURE_STATE_ACTION when a tap gesture is detected
 *
 */
class TapDetector: public GestureDetector
{
private:
    int32_t down_pointer_id_;

public:
    float down_x_;
    float down_y_;
    
    TapDetector()
    {
    }
    virtual ~TapDetector()
    {
    }
    virtual GESTURE_STATE Detect( const AInputEvent* motion_event )
    {
        if( AMotionEvent_getPointerCount( motion_event ) > 1 )
        {
            //Only support single touch
            return false;
        }
        
        int32_t action = AMotionEvent_getAction( motion_event );
        unsigned int flags = action & AMOTION_EVENT_ACTION_MASK;
        switch( flags )
        {
            case AMOTION_EVENT_ACTION_DOWN:
                down_pointer_id_ = AMotionEvent_getPointerId( motion_event, 0 );
                down_x_ = AMotionEvent_getX( motion_event, 0 );
                down_y_ = AMotionEvent_getY( motion_event, 0 );
                break;
            case AMOTION_EVENT_ACTION_UP:
            {
                int64_t eventTime = AMotionEvent_getEventTime( motion_event );
                int64_t downTime = AMotionEvent_getDownTime( motion_event );
                if( eventTime - downTime <= TAP_TIMEOUT )
                {
                    if( down_pointer_id_ == AMotionEvent_getPointerId( motion_event, 0 ) )
                    {
                        float x = AMotionEvent_getX( motion_event, 0 ) - down_x_;
                        float y = AMotionEvent_getY( motion_event, 0 ) - down_y_;
                        if( x * x + y * y < TOUCH_SLOP * TOUCH_SLOP * dp_factor_ )
                        {
                            LOGI( "TapDetector: Tap detected" );
                            return GESTURE_STATE_ACTION;
                        }
                    }
                }
                break;
            }
        }
        return GESTURE_STATE_NONE;
    }
};

/******************************************************************
 * Drag gesture detector
 * Returns drag gesture state when a drag-tap gesture is detected
 *
 */
class DragDetector: public GestureDetector
{
private:
    int32_t FindIndex (const AInputEvent* event, int32_t id)
    {
        int32_t count = AMotionEvent_getPointerCount(event);
        for (uint32_t i = 0; i < count; ++i)
        {
            if (id == AMotionEvent_getPointerId( event, i ))
            {
                return i;
            }
        }
        
        return -1;
    }
    
    const AInputEvent* event_;
    std::vector<int32_t> vec_pointers_;
public:
    DragDetector()
    {
    }
    
    virtual ~DragDetector()
    {
    }
    
    virtual GESTURE_STATE Detect(const AInputEvent* event)
    {
        GESTURE_STATE ret = GESTURE_STATE_NONE;
        int32_t action = AMotionEvent_getAction( event );
        int32_t index = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK)
        >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
        uint32_t flags = action & AMOTION_EVENT_ACTION_MASK;
        event_ = event;
        
        int32_t count = AMotionEvent_getPointerCount( event );
        switch( flags )
        {
            case AMOTION_EVENT_ACTION_DOWN:
                vec_pointers_.push_back( AMotionEvent_getPointerId( event, 0 ) );
                ret = GESTURE_STATE_START;
                break;
            case AMOTION_EVENT_ACTION_POINTER_DOWN:
                vec_pointers_.push_back( AMotionEvent_getPointerId( event, index ) );
                break;
            case AMOTION_EVENT_ACTION_UP:
                vec_pointers_.pop_back();
                ret = GESTURE_STATE_END;
                break;
            case AMOTION_EVENT_ACTION_POINTER_UP:
            {
                int32_t released_pointer_id = AMotionEvent_getPointerId( event, index );
                
                std::vector<int32_t>::iterator it = vec_pointers_.begin();
                std::vector<int32_t>::iterator it_end = vec_pointers_.end();
                int32_t i = 0;
                for( ; it != it_end; ++it, ++i )
                {
                    if( *it == released_pointer_id )
                    {
                        vec_pointers_.erase( it );
                        break;
                    }
                }
                
                if (i <= 1)
                {
                    //Reset pinch or drag
                    if( count == 2 )
                    {
                        ret = GESTURE_STATE_START;
                    }
                }
                break;
            }
            case AMOTION_EVENT_ACTION_MOVE:
                switch( count )
            {
                case 1:
                    //Drag
                    ret = GESTURE_STATE_MOVE;
                    break;
                default:
                    break;
            }
                break;
            case AMOTION_EVENT_ACTION_CANCEL:
                break;
        }
        
        return ret;
    }
    
    bool GetPointer(Vector2D& v)
    {
        if (vec_pointers_.size() < 1)
        {
            return false;
        }
        
        int32_t iIndex = FindIndex(event_, vec_pointers_[0]);
        if (iIndex == -1)
        {
            return false;
        }
        
        float x = AMotionEvent_getX(event_, iIndex);
        float y = AMotionEvent_getY(event_, iIndex);
        
        v.set(x, y);
        
        return true;
    }
};

//-------------------------------------------------------------------------
//Preprocessor
//-------------------------------------------------------------------------
#define HELPER_CLASS_NAME "com/noctisgames/nosfuratu/NDKHelper" //Class name of helper function
//-------------------------------------------------------------------------
//Shared state for our app.
//-------------------------------------------------------------------------
struct android_app;
class Engine
{
    ndk_helper::GLContext* gl_context_;
    
    bool initialized_resources_;
    bool has_focus_;
    float m_fLastTime;
    
    TapDetector tap_detector_;
    DragDetector drag_detector_;
    ndk_helper::PerfMonitor monitor_;
    
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
    
    m_fLastTime = monitor_.GetCurrentTime();
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
    
    if (MAIN_ASSETS->isUsingCompressedTextureSet())
    {
        m_screen->createWindowSizeDependentResources(width > 1440 ? 1440 : width, height > 900 ? 900 : height, width, height);
    }
    else
    {
        m_screen->createWindowSizeDependentResources(width > 1024 ? 1024 : width, height > 576 ? 576 : height, width, height);
    }
    
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
    float fFPS;
    if( monitor_.Update( fFPS ) )
    {
        UpdateFPS( fFPS );
    }
    
    LOGI("NOSFURATU XXXX YYY DrawFrame %f", m_fLastTime);
    
    float time = monitor_.GetCurrentTime();
    float deltaTime = time - m_fLastTime;
    m_fLastTime = time;
    
    LOGI("NOSFURATU XXXX YYY DrawFrame %f", deltaTime);
    LOGI("NOSFURATU XXXX YYY DrawFrame %f", time);
    
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
    
    m_screen->update(deltaTime);
    
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
        GESTURE_STATE tapState = eng->tap_detector_.Detect(event);
        
        if (tapState & GESTURE_STATE_ACTION)
        {
            SCREEN_INPUT_MANAGER->onTouch(ScreenEventType_UP, eng->tap_detector_.down_x_, eng->tap_detector_.down_y_);
        }
        else
        {
            static Vector2D touchInput;
            
            GESTURE_STATE dragState = eng->drag_detector_.Detect(event);
            
            //Handle drag state
            if (dragState & GESTURE_STATE_START)
            {
                //Otherwise, start dragging
                eng->drag_detector_.GetPointer(touchInput);
                SCREEN_INPUT_MANAGER->onTouch(ScreenEventType_DOWN, touchInput.getX(), touchInput.getY());
            }
            else if (dragState & GESTURE_STATE_MOVE)
            {
                eng->drag_detector_.GetPointer(touchInput);
                SCREEN_INPUT_MANAGER->onTouch(ScreenEventType_DRAGGED, touchInput.getX(), touchInput.getY());
            }
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
    tap_detector_.SetConfiguration(app_->config);
    drag_detector_.SetConfiguration(app_->config);
}

bool Engine::IsReady()
{
    if (has_focus_)
    {
        return true;
    }
    
    return false;
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
