//
//  game.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "macros.h"
#include "MainScreen.h"
#include "MainScreenTitle.h"
#include "ScreenInputManager.h"
#include "KeyboardInputManager.h"
#include "AndroidAudioEngineHelper.h"
#include "AndroidAssetDataHandler.h"
#include "MainAssets.h"

#include <jni.h>

MainScreen* gScreen;

/* These functions are called from Java. */
extern "C"
{
    JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_init(JNIEnv* env, jclass cls, jobject activity, jboolean isLowMemoryDevice);
    
    JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_on_1surface_1created(JNIEnv * env, jclass cls);
    
    JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_on_1surface_1changed(JNIEnv * env, jclass cls, jint pixel_width, jint pixel_height);
    
    JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_on_1resume(JNIEnv* env, jclass cls);
    
    JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_on_1pause(JNIEnv* env, jclass cls);
    
    JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_on_1stop(JNIEnv* env, jclass cls);
    
    JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_update(JNIEnv* env, jclass cls, jfloat delta_time);
    
    JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_render(JNIEnv* env, jclass cls);
    
    JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_on_1touch_1down(JNIEnv* env, jclass cls, jfloat raw_touch_x, jfloat raw_touch_y);
    
    JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_on_1touch_1dragged(JNIEnv* env, jclass cls, jfloat raw_touch_x, jfloat raw_touch_y);
    
    JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_on_1touch_1up(JNIEnv* env, jclass cls, jfloat raw_touch_x, jfloat raw_touch_y);
    
    JNIEXPORT int JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_get_1requested_1action(JNIEnv* env, jclass cls);
    
    JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_clear_1requested_1action(JNIEnv* env, jclass cls);
    
    JNIEXPORT bool JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_handle_1on_1back_1pressed(JNIEnv* env, jclass cls);
};

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_init(JNIEnv* env, jclass cls, jobject activity, jboolean isLowMemoryDevice)
{
    UNUSED(cls);
    
    ANDROID_AUDIO_ENGINE_HELPER->init(env, activity);
    
    ANDROID_ASSETS->init(env, activity);
    
    MAIN_ASSETS->setUsingCompressedTextureSet(isLowMemoryDevice);
    
    gScreen = new MainScreen();
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_on_1surface_1created(JNIEnv * env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);

    gScreen->releaseDeviceDependentResources();
    
    gScreen->createDeviceDependentResources();
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_on_1surface_1changed(JNIEnv * env, jclass cls, jint pixel_width, jint pixel_height)
{
	UNUSED(env);
	UNUSED(cls);

    gScreen->createWindowSizeDependentResources(pixel_width, pixel_height, pixel_width, pixel_height);
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_on_1resume(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);

    gScreen->onResume();
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_on_1pause(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);

    gScreen->onPause();
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_on_1stop(JNIEnv* env, jclass cls)
{
    UNUSED(env);
    UNUSED(cls);
    
    ANDROID_AUDIO_ENGINE_HELPER->deinit();
    
    delete gScreen;
    gScreen = nullptr;
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_update(JNIEnv* env, jclass cls, jfloat delta_time)
{
	UNUSED(env);
	UNUSED(cls);

    gScreen->update(delta_time);
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_render(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);

    gScreen->render();
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_on_1touch_1down(JNIEnv* env, jclass cls, jfloat raw_touch_x, jfloat raw_touch_y)
{
	UNUSED(env);
	UNUSED(cls);
    
    SCREEN_INPUT_MANAGER->onTouch(ScreenEventType_DOWN, raw_touch_x, raw_touch_y);
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_on_1touch_1dragged(JNIEnv* env, jclass cls, jfloat raw_touch_x, jfloat raw_touch_y)
{
	UNUSED(env);
	UNUSED(cls);
    
    SCREEN_INPUT_MANAGER->onTouch(ScreenEventType_DRAGGED, raw_touch_x, raw_touch_y);
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_on_1touch_1up(JNIEnv* env, jclass cls, jfloat raw_touch_x, jfloat raw_touch_y)
{
	UNUSED(env);
	UNUSED(cls);
    
    SCREEN_INPUT_MANAGER->onTouch(ScreenEventType_UP, raw_touch_x, raw_touch_y);
}

JNIEXPORT int JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_get_1requested_1action(JNIEnv* env, jclass cls)
{
	UNUSED(env);
    UNUSED(cls);
    
	return gScreen->getRequestedAction();
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_clear_1requested_1action(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);

	gScreen->clearRequestedAction();
}

JNIEXPORT bool JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_handle_1on_1back_1pressed(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);
    
    if (gScreen->m_stateMachine.getCurrentState() == Title::getInstance())
    {
        return false;
    }
    
    KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_BACK, true);
    
	return true;
}
