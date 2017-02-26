//
//  game.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include <jni.h>
#include <stdio.h>

#include "macros.h"
#include "MainScreen.h"
#include "MainScreenWorldMap.h"
#include "ScreenInputManager.h"
#include "KeyboardInputManager.h"
#include "SoundManager.h"
#include "MainAssets.h"
#include "MainScreenTitle.h"
#include "AndroidAssetsManager.h"

MainScreen *screen;

/* These functions are called from Java. */
extern "C"
{
    JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_init(JNIEnv* env, jclass cls, jobject java_asset_manager, jboolean isLowMemoryDevice);
    
    JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_on_1surface_1created(JNIEnv * env, jclass cls);
    
    JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_on_1surface_1changed(JNIEnv * env, jclass cls, jint pixel_width, jint pixel_height);
    
    JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_on_1resume(JNIEnv* env, jclass cls);
    
    JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_on_1pause(JNIEnv* env, jclass cls);
    
    JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_update(JNIEnv* env, jclass cls, jfloat delta_time);
    
    JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_render(JNIEnv* env, jclass cls);
    
    JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_on_1touch_1down(JNIEnv* env, jclass cls, jfloat raw_touch_x, jfloat raw_touch_y);
    
    JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_on_1touch_1dragged(JNIEnv* env, jclass cls, jfloat raw_touch_x, jfloat raw_touch_y);
    
    JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_on_1touch_1up(JNIEnv* env, jclass cls, jfloat raw_touch_x, jfloat raw_touch_y);
    
    JNIEXPORT short JNICALL Java_com_noctisgames_nosfuratu_Game_get_1current_1music_1id(JNIEnv* env, jclass cls);
    
    JNIEXPORT short JNICALL Java_com_noctisgames_nosfuratu_Game_get_1current_1sound_1id(JNIEnv* env, jclass cls);
    
    JNIEXPORT int JNICALL Java_com_noctisgames_nosfuratu_Game_get_1requested_1action(JNIEnv* env, jclass cls);
    
    JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_clear_1requested_1action(JNIEnv* env, jclass cls);
    
    JNIEXPORT bool JNICALL Java_com_noctisgames_nosfuratu_Game_handle_1on_1back_1pressed(JNIEnv* env, jclass cls);
    
    JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_load_1user_1save_1data(JNIEnv *env, jclass cls, jstring json);
    
    JNIEXPORT int JNICALL Java_com_noctisgames_nosfuratu_Game_get_1score(JNIEnv* env, jclass cls);
    
    JNIEXPORT int JNICALL Java_com_noctisgames_nosfuratu_Game_get_1online_1score(JNIEnv* env, jclass cls);
    
    JNIEXPORT int JNICALL Java_com_noctisgames_nosfuratu_Game_get_1level_1stats_1flag(JNIEnv* env, jclass cls);
    
    JNIEXPORT int JNICALL Java_com_noctisgames_nosfuratu_Game_get_1num_1golden_1carrots(JNIEnv* env, jclass cls);
    
    JNIEXPORT int JNICALL Java_com_noctisgames_nosfuratu_Game_get_1jon_1unlocked_1abilities_1flag(JNIEnv* env, jclass cls);
    
    JNIEXPORT int JNICALL Java_com_noctisgames_nosfuratu_Game_get_1level_1stats_1flag_1for_1unlocked_1level(JNIEnv* env, jclass cls);
    
    JNIEXPORT int JNICALL Java_com_noctisgames_nosfuratu_Game_get_1num_1golden_1carrots_1after_1unlocking_1level(JNIEnv* env, jclass cls);
};

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_init(JNIEnv* env, jclass cls, jobject java_asset_manager, jboolean isLowMemoryDevice)
{
	UNUSED(env);
	UNUSED(cls);

    ANDROID_ASSETS->init(env, java_asset_manager);
    
    MAIN_ASSETS->setUsingCompressedTextureSet(isLowMemoryDevice);
    
    screen = new MainScreen();
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_on_1surface_1created(JNIEnv * env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);

    screen->releaseDeviceDependentResources();
    
    screen->createDeviceDependentResources();
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_on_1surface_1changed(JNIEnv * env, jclass cls, jint pixel_width, jint pixel_height)
{
	UNUSED(env);
	UNUSED(cls);

    screen->createWindowSizeDependentResources(pixel_width, pixel_height, pixel_width, pixel_height);
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_on_1resume(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);

    screen->onResume();
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_on_1pause(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);

    screen->onPause();
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_update(JNIEnv* env, jclass cls, jfloat delta_time)
{
	UNUSED(env);
	UNUSED(cls);

    screen->update(delta_time);
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_render(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);

    screen->render();
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_on_1touch_1down(JNIEnv* env, jclass cls, jfloat raw_touch_x, jfloat raw_touch_y)
{
	UNUSED(env);
	UNUSED(cls);
    
    SCREEN_INPUT_MANAGER->onTouch(ScreenEventType_DOWN, raw_touch_x, raw_touch_y);
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_on_1touch_1dragged(JNIEnv* env, jclass cls, jfloat raw_touch_x, jfloat raw_touch_y)
{
	UNUSED(env);
	UNUSED(cls);
    
    SCREEN_INPUT_MANAGER->onTouch(ScreenEventType_DRAGGED, raw_touch_x, raw_touch_y);
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_on_1touch_1up(JNIEnv* env, jclass cls, jfloat raw_touch_x, jfloat raw_touch_y)
{
	UNUSED(env);
	UNUSED(cls);
    
    SCREEN_INPUT_MANAGER->onTouch(ScreenEventType_UP, raw_touch_x, raw_touch_y);
}

JNIEXPORT short JNICALL Java_com_noctisgames_nosfuratu_Game_get_1current_1music_1id(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);
    
    return SOUND_MANAGER->getCurrentMusicId();
}

JNIEXPORT short JNICALL Java_com_noctisgames_nosfuratu_Game_get_1current_1sound_1id(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);

	return SOUND_MANAGER->getCurrentSoundId();
}

JNIEXPORT int JNICALL Java_com_noctisgames_nosfuratu_Game_get_1requested_1action(JNIEnv* env, jclass cls)
{
	UNUSED(env);
    UNUSED(cls);
    
	return screen->getRequestedAction();
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_clear_1requested_1action(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);

	screen->clearRequestedAction();
}

JNIEXPORT bool JNICALL Java_com_noctisgames_nosfuratu_Game_handle_1on_1back_1pressed(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);
    
    if (screen->m_stateMachine.getCurrentState() == Title::getInstance())
    {
        return false;
    }
    
    KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_BACK, true);
    
	return true;
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_load_1user_1save_1data(JNIEnv *env, jclass cls, jstring json)
{
	UNUSED(env);
	UNUSED(cls);

	const char *nativeJson = (env)->GetStringUTFChars(json, nullptr);

	WorldMap::getInstance()->loadUserSaveData(nativeJson);
}

JNIEXPORT int JNICALL Java_com_noctisgames_nosfuratu_Game_get_1score(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);
    
	return screen->getScore();
}

JNIEXPORT int JNICALL Java_com_noctisgames_nosfuratu_Game_get_1online_1score(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);
    
	return screen->getOnlineScore();
}

JNIEXPORT int JNICALL Java_com_noctisgames_nosfuratu_Game_get_1level_1stats_1flag(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);
    
	return screen->getLevelStatsFlag();
}

JNIEXPORT int JNICALL Java_com_noctisgames_nosfuratu_Game_get_1num_1golden_1carrots(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);
    
	return screen->getNumGoldenCarrots();
}

JNIEXPORT int JNICALL Java_com_noctisgames_nosfuratu_Game_get_1jon_1unlocked_1abilities_1flag(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);
    
	return screen->getJonAbilityFlag();
}

JNIEXPORT int JNICALL Java_com_noctisgames_nosfuratu_Game_get_1level_1stats_1flag_1for_1unlocked_1level(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);
    
	return screen->getLevelStatsFlagForUnlockedLevel();
}

JNIEXPORT int JNICALL Java_com_noctisgames_nosfuratu_Game_get_1num_1golden_1carrots_1after_1unlocking_1level(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);
    
	return screen->getNumGoldenCarrotsAfterUnlockingLevel();
}
