//
//  MainNativeActivity.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 3/18/17.
//  Copyright © 2017 Noctis Games. All rights reserved.
//

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
#include "MathUtil.h"

#include "NDKHelper.h"

#include <jni.h>
#include <errno.h>
#include <chrono>
#include <vector>
#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/native_window_jni.h>
#include <cpu-features.h>

struct android_app;

class Engine
{
public:
    static void handleCmd(struct android_app* app, int32_t cmd);
    static int32_t handleInput(android_app* app, AInputEvent* event);
    
    Engine();
    ~Engine();
    
    void onSignInSucceeded();
    void onSignInFailed();
    
    void setState(android_app* state);
    int initDisplay();
    void loadResources();
    void unloadResources();
    void drawFrame();
    void termDisplay();
    void trimMemory();
    bool isReady();
    
    void resume();
    void pause();
    
private:
    ndk_helper::GLContext* m_glContext;
    android_app* m_app;
    MainScreen* m_screen;
    
    float m_fAveragedDeltaTime;
    bool m_hasInitializedResources;
    bool m_hasFocus;
    bool m_needsToSubmitScoreAfterAuthentication;
    
    void initializeInterstitialAds();
    void displayInterstitialAdIfLoaded();
    bool isSignedIn();
    void beginUserInitiatedSignIn();
    void signOut();
    void showAchievements();
    void showLeaderboards();
    void unlockAchievement(const char* key);
    void submitScore(const char* key, int score);
    void submitAndDisplayScore(const char* key, int score);
    std::string getStringResource(const std::string& resourceName);
};

void Engine::handleCmd(struct android_app* app, int32_t cmd)
{
    Engine* eng = (Engine*) app->userData;
    switch (cmd)
    {
        case APP_CMD_SAVE_STATE:
            LOGI("NOSFURATU APP_CMD_SAVE_STATE");
            break;
        case APP_CMD_INIT_WINDOW:
            LOGI("NOSFURATU APP_CMD_INIT_WINDOW");
            if (app->window != NULL)
            {
                eng->initDisplay();
                eng->drawFrame();
                eng->initializeInterstitialAds();
            }
            break;
        case APP_CMD_TERM_WINDOW:
            LOGI("NOSFURATU APP_CMD_TERM_WINDOW");
            eng->termDisplay();
            eng->m_hasFocus = false;
            break;
        case APP_CMD_DESTROY:
            LOGI("NOSFURATU APP_CMD_DESTROY");
            break;
        case APP_CMD_STOP:
            LOGI("NOSFURATU APP_CMD_STOP");
            break;
        case APP_CMD_GAINED_FOCUS:
            LOGI("NOSFURATU APP_CMD_GAINED_FOCUS");
            eng->resume();
            eng->m_hasFocus = true;
            break;
        case APP_CMD_LOST_FOCUS:
            LOGI("NOSFURATU APP_CMD_LOST_FOCUS");
            eng->pause();
            eng->m_hasFocus = false;
            eng->drawFrame();
            break;
        case APP_CMD_LOW_MEMORY:
            LOGI("NOSFURATU APP_CMD_LOW_MEMORY");
            eng->trimMemory();
            break;
        case APP_CMD_RESUME:
            LOGI("NOSFURATU APP_CMD_RESUME");
            eng->resume();
            break;
        case APP_CMD_PAUSE:
            LOGI("NOSFURATU APP_CMD_PAUSE");
            eng->pause();
            break;
        case APP_CMD_INPUT_CHANGED:
            LOGI("NOSFURATU APP_CMD_INPUT_CHANGED");
            break;
        case APP_CMD_WINDOW_RESIZED:
            LOGI("NOSFURATU APP_CMD_WINDOW_RESIZED");
            break;
        case APP_CMD_WINDOW_REDRAW_NEEDED:
            LOGI("NOSFURATU APP_CMD_WINDOW_REDRAW_NEEDED");
            break;
        case APP_CMD_CONTENT_RECT_CHANGED:
            LOGI("NOSFURATU APP_CMD_CONTENT_RECT_CHANGED");
            break;
        case APP_CMD_CONFIG_CHANGED:
            LOGI("NOSFURATU APP_CMD_CONFIG_CHANGED");
            break;
        case APP_CMD_START:
            LOGI("NOSFURATU APP_CMD_START");
            break;
    }
}

int32_t Engine::handleInput(android_app* app, AInputEvent* event)
{
    Engine* eng = (Engine*) app->userData;
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
    {
        int32_t action = AMotionEvent_getAction(event);
        int32_t pointerIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
        uint32_t flags = action & AMOTION_EVENT_ACTION_MASK;
        
        int32_t pointerCount = AMotionEvent_getPointerCount(event);
        
        switch (flags)
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
    else if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_KEY)
    {
        if (AKeyEvent_getKeyCode(event) == AKEYCODE_BACK
            && eng->m_screen->m_stateMachine.getCurrentState() != Title::getInstance())
        {
            KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_BACK, true);
            
            return 1; // <-- prevent default handler
        }
    }
    
    return 0;
}

Engine::Engine() :
m_glContext(ndk_helper::GLContext::GetInstance()),
m_app(nullptr),
m_screen(nullptr),
m_fAveragedDeltaTime(0.016666666666667f),
m_hasInitializedResources(false),
m_hasFocus(false),
m_needsToSubmitScoreAfterAuthentication(false)
{
    // Empty
}

Engine::~Engine()
{
}

void Engine::setState(android_app* state)
{
    m_app = state;
}

int Engine::initDisplay()
{
    if (!m_hasInitializedResources)
    {
        m_glContext->Init(m_app->window);
        
        loadResources();
        m_hasInitializedResources = true;
    }
    else
    {
        // initialize OpenGL ES and EGL
        if (EGL_SUCCESS != m_glContext->Resume(m_app->window))
        {
            unloadResources();
            loadResources();
        }
        
        resume();
    }
    
    return 0;
}

void Engine::loadResources()
{
    JNIEnv *jni;
    m_app->activity->vm->AttachCurrentThread(&jni, NULL);
    
    ANDROID_AUDIO_ENGINE_HELPER->init(jni, m_app->activity->clazz);
    
    ANDROID_ASSETS->init(jni, m_app->activity->clazz);
    
    MAIN_ASSETS->setUsingCompressedTextureSet(m_glContext->GetScreenWidth() < 2560);
    
    if (!m_screen)
    {
        m_screen = new MainScreen();
    }
    
    int width = m_glContext->GetScreenWidth();
    int height = m_glContext->GetScreenHeight();
    
    m_screen->createDeviceDependentResources();
    
    OGLManager->setScreenSize(width, height);
    
    if (MAIN_ASSETS->isUsingCompressedTextureSet())
    {
        m_screen->createWindowSizeDependentResources(width > 1280 ? 1280 : width, height > 720 ? 720 : height, width, height);
    }
    else
    {
        m_screen->createWindowSizeDependentResources(width > 1440 ? 1440 : width, height > 900 ? 900 : height, width, height);
    }
    
    m_app->activity->vm->DetachCurrentThread();
    return;
}

void Engine::unloadResources()
{
    ANDROID_AUDIO_ENGINE_HELPER->deinit();
    
    ANDROID_ASSETS->deinit();
    
    m_screen->releaseDeviceDependentResources();
}

std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();

inline float approxRollingAverage(float avg, float newSample)
{
    static float numSamples = 60;
    
    avg -= avg / numSamples;
    avg += newSample / numSamples;
    
    return avg;
}

void Engine::drawFrame()
{
    float deltaTime;
    
    using namespace std;
    using namespace std::chrono;
    
    t2 = steady_clock::now();
    duration<float> timeSpan = duration_cast<duration<float>>(t2 - t1);
    t1 = steady_clock::now();
    
    deltaTime = timeSpan.count();
    
    m_fAveragedDeltaTime = approxRollingAverage(m_fAveragedDeltaTime, deltaTime);
    
    deltaTime = clamp(m_fAveragedDeltaTime, 0.016f, 0.033f);
    
    if (m_fAveragedDeltaTime > 0.016f && m_fAveragedDeltaTime < 0.018f)
    {
        deltaTime = 0.016666666666667f;
    }
    
    bool signedIn = isSignedIn();
    if (m_needsToSubmitScoreAfterAuthentication
        && !m_screen->m_isAuthenticated
        && signedIn)
    {
        std::string key = m_screen->getLeaderboardKey();
        std::string id = getStringResource(key);
        int score = m_screen->getScore();
        submitAndDisplayScore(id.c_str(), score);
        
        m_needsToSubmitScoreAfterAuthentication = false;
    }
    
    m_screen->setAuthenticated(signedIn);
    
    int requestedAction = m_screen->getRequestedAction();
    
    switch (requestedAction)
    {
        case REQUESTED_ACTION_DISPLAY_INTERSTITIAL_AD:
            displayInterstitialAdIfLoaded();
            m_screen->clearRequestedAction();
            break;
        case REQUESTED_ACTION_SUBMIT_SCORE_TO_LEADERBOARD:
        {
            if (isSignedIn())
            {
                std::string key = m_screen->getLeaderboardKey();
                std::string id = getStringResource(key);
                int score = m_screen->getScore();
                submitScore(id.c_str(), score);
            }
        }
            m_screen->clearRequestedAction();
            break;
        case REQUESTED_ACTION_SUBMIT_AND_DISPLAY_SCORE_TO_LEADERBOARD:
        {
            if (isSignedIn())
            {
                std::string key = m_screen->getLeaderboardKey();
                std::string id = getStringResource(key);
                int score = m_screen->getScore();
                submitAndDisplayScore(id.c_str(), score);
            }
            else
            {
                m_needsToSubmitScoreAfterAuthentication = true;
                beginUserInitiatedSignIn();
            }
        }
            m_screen->clearRequestedAction();
            break;
        case REQUESTED_ACTION_UNLOCK_ACHIEVEMENT:
        {
            if (isSignedIn())
            {
                std::vector<std::string>& keys = m_screen->getUnlockedAchievementsKeys();
                for (std::vector<std::string>::iterator i = keys.begin(); i != keys.end(); ++i)
                {
                    std::string key = *i;
                    std::string id = getStringResource(key);
                    unlockAchievement(id.c_str());
                }
            }
        }
            m_screen->clearRequestedAction();
            break;
        case REQUESTED_ACTION_DISPLAY_LEADERBOARDS:
            if (isSignedIn())
            {
                showLeaderboards();
            }
            else
            {
                beginUserInitiatedSignIn();
            }
            m_screen->clearRequestedAction();
            break;
        case REQUESTED_ACTION_DISPLAY_ACHIEVEMENTS:
            if (isSignedIn())
            {
                showAchievements();
            }
            else
            {
                beginUserInitiatedSignIn();
            }
            m_screen->clearRequestedAction();
            break;
        case REQUESTED_ACTION_SIGN_IN:
            if (!isSignedIn())
            {
                beginUserInitiatedSignIn();
            }
            m_screen->clearRequestedAction();
            break;
        case REQUESTED_ACTION_SIGN_OUT:
            if (isSignedIn())
            {
                signOut();
            }
            m_screen->clearRequestedAction();
            break;
        case REQUESTED_ACTION_UPDATE:
            break;
        default:
            m_screen->clearRequestedAction();
            break;
    }
    
    m_screen->update(deltaTime);
    
    m_screen->render();
    
    if (EGL_SUCCESS != m_glContext->Swap())
    {
        unloadResources();
        loadResources();
    }
}

void Engine::termDisplay()
{
    pause();
    
    m_glContext->Suspend();
}

void Engine::trimMemory()
{
    m_glContext->Invalidate();
}

bool Engine::isReady()
{
    if (m_hasFocus)
    {
        return true;
    }
    
    return false;
}

void Engine::resume()
{
    if (m_screen)
    {
        m_screen->onResume();
        
        m_hasFocus = true;
    }
}

void Engine::pause()
{
    if (m_screen)
    {
        m_screen->onPause();
        
        m_hasFocus = false;
    }
}

void Engine::initializeInterstitialAds()
{
    JNIEnv *jni;
    m_app->activity->vm->AttachCurrentThread( &jni, NULL );
    
    //Default class retrieval
    jclass clazz = jni->GetObjectClass(m_app->activity->clazz);
    jmethodID methodID = jni->GetMethodID(clazz, "initializeInterstitialAds", "()V");
    jni->CallVoidMethod(m_app->activity->clazz, methodID);
    
    m_app->activity->vm->DetachCurrentThread();
}

void Engine::displayInterstitialAdIfLoaded()
{
    JNIEnv *jni;
    m_app->activity->vm->AttachCurrentThread( &jni, NULL );
    
    //Default class retrieval
    jclass clazz = jni->GetObjectClass(m_app->activity->clazz);
    jmethodID methodID = jni->GetMethodID(clazz, "displayInterstitialAdIfLoaded", "()V");
    jni->CallVoidMethod(m_app->activity->clazz, methodID);
    
    m_app->activity->vm->DetachCurrentThread();
}

bool Engine::isSignedIn()
{
    JNIEnv *jni;
    m_app->activity->vm->AttachCurrentThread( &jni, NULL );
    
    jclass clazz = jni->GetObjectClass(m_app->activity->clazz);
    jmethodID methodID = jni->GetMethodID(clazz, "isSignedIn", "()Z");
    jboolean ret = jni->CallBooleanMethod(m_app->activity->clazz, methodID);
    
    m_app->activity->vm->DetachCurrentThread();
    
    return ret;
}

void Engine::beginUserInitiatedSignIn()
{
    JNIEnv *jni;
    m_app->activity->vm->AttachCurrentThread( &jni, NULL );
    
    //Default class retrieval
    jclass clazz = jni->GetObjectClass(m_app->activity->clazz);
    jmethodID methodID = jni->GetMethodID(clazz, "beginUserInitiatedSignIn", "()V");
    jni->CallVoidMethod(m_app->activity->clazz, methodID);
    
    m_app->activity->vm->DetachCurrentThread();
}

void Engine::signOut()
{
    JNIEnv *jni;
    m_app->activity->vm->AttachCurrentThread( &jni, NULL );
    
    //Default class retrieval
    jclass clazz = jni->GetObjectClass(m_app->activity->clazz);
    jmethodID methodID = jni->GetMethodID(clazz, "signOut", "()V");
    jni->CallVoidMethod(m_app->activity->clazz, methodID);
    
    m_app->activity->vm->DetachCurrentThread();
}

void Engine::showAchievements()
{
    JNIEnv *jni;
    m_app->activity->vm->AttachCurrentThread( &jni, NULL );
    
    //Default class retrieval
    jclass clazz = jni->GetObjectClass(m_app->activity->clazz);
    jmethodID methodID = jni->GetMethodID(clazz, "showAchievements", "()V");
    jni->CallVoidMethod(m_app->activity->clazz, methodID);
    
    m_app->activity->vm->DetachCurrentThread();
}

void Engine::showLeaderboards()
{
    JNIEnv *jni;
    m_app->activity->vm->AttachCurrentThread( &jni, NULL );
    
    //Default class retrieval
    jclass clazz = jni->GetObjectClass(m_app->activity->clazz);
    jmethodID methodID = jni->GetMethodID(clazz, "showLeaderboards", "()V");
    jni->CallVoidMethod(m_app->activity->clazz, methodID);
    
    m_app->activity->vm->DetachCurrentThread();
}

void Engine::unlockAchievement(const char* key)
{
    JNIEnv *jni;
    m_app->activity->vm->AttachCurrentThread( &jni, NULL );
    
    jstring java_key = jni->NewStringUTF(key);
    
    //Default class retrieval
    jclass clazz = jni->GetObjectClass(m_app->activity->clazz);
    jmethodID methodID = jni->GetMethodID(clazz, "unlockAchievement", "(Ljava/lang/String;)V");
    jni->CallVoidMethod(m_app->activity->clazz, methodID, java_key);
    
    jni->DeleteLocalRef(java_key);
    
    m_app->activity->vm->DetachCurrentThread();
}

void Engine::submitScore(const char* key, int score)
{
    JNIEnv *jni;
    m_app->activity->vm->AttachCurrentThread( &jni, NULL );
    
    jstring java_key = jni->NewStringUTF(key);
    
    //Default class retrieval
    jclass clazz = jni->GetObjectClass(m_app->activity->clazz);
    jmethodID methodID = jni->GetMethodID(clazz, "submitScore", "(Ljava/lang/String;I)V");
    jni->CallVoidMethod(m_app->activity->clazz, methodID, java_key, score);
    
    jni->DeleteLocalRef(java_key);
    
    m_app->activity->vm->DetachCurrentThread();
}

void Engine::submitAndDisplayScore(const char* key, int score)
{
    JNIEnv *jni;
    m_app->activity->vm->AttachCurrentThread( &jni, NULL );
    
    jstring java_key = jni->NewStringUTF(key);
    
    //Default class retrieval
    jclass clazz = jni->GetObjectClass(m_app->activity->clazz);
    jmethodID methodID = jni->GetMethodID(clazz, "submitAndDisplayScore", "(Ljava/lang/String;I)V");
    jni->CallVoidMethod(m_app->activity->clazz, methodID, java_key, score);
    
    jni->DeleteLocalRef(java_key);
    
    m_app->activity->vm->DetachCurrentThread();
}

std::string Engine::getStringResource(const std::string& resourceName)
{
    JNIEnv *jni;
    m_app->activity->vm->AttachCurrentThread( &jni, NULL );
    
    jstring name = jni->NewStringUTF(resourceName.c_str());
    
    jclass clazz = jni->GetObjectClass(m_app->activity->clazz);
    jmethodID methodID = jni->GetMethodID(clazz, "getStringResource", "(Ljava/lang/String;)Ljava/lang/String;");
    jstring ret = (jstring) jni->CallObjectMethod(m_app->activity->clazz, methodID, name);
    
    const char *resource = jni->GetStringUTFChars(ret, NULL);
    std::string s = std::string(resource);
    
    jni->ReleaseStringUTFChars(ret, resource);
    jni->DeleteLocalRef(ret);
    jni->DeleteLocalRef(name);
    
    m_app->activity->vm->DetachCurrentThread();
    
    return s;
}

Engine g_engine;

/**
 * This is the main entry point of a native application that is using
 * android_native_m_appglue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(android_app* state)
{
    app_dummy();
    
    g_engine.setState(state);
    
    state->userData = &g_engine;
    state->onAppCmd = Engine::handleCmd;
    state->onInputEvent = Engine::handleInput;
    
#ifdef USE_NDK_PROFILER
    monstartup("libandroid_main.so");
#endif
    
    while (1)
    {
        // Read all pending events.
        int id;
        int events;
        android_poll_source* source;
        
        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((id = ALooper_pollAll(g_engine.isReady() ? 0 : -1, NULL, &events, (void**) &source)) >= 0)
        {
            if (source != NULL)
            {
                source->process(state, source);
            }
            
            if (state->destroyRequested != 0)
            {
                g_engine.termDisplay();
                return;
            }
        }
        
        if (g_engine.isReady())
        {
            g_engine.drawFrame();
        }
    }
}
