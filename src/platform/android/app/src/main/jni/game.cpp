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
#include "AndroidOpenGLESGameScreen.h"
#include "GameScreenLevelEditor.h"

AndroidOpenGLESGameScreen *gameScreen;

/* These functions are called from Java. */
extern "C"
{
JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_init(JNIEnv* env, jclass cls);

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

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_load_1level(JNIEnv* env, jclass cls, jstring level_json);

JNIEXPORT bool JNICALL Java_com_noctisgames_nosfuratu_Game_save_1level(JNIEnv *env, jclass cls, jstring json_file_path);
};

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_init(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);

	gameScreen = new AndroidOpenGLESGameScreen();
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_on_1surface_1created(JNIEnv * env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);

	gameScreen->onSurfaceCreated();
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_on_1surface_1changed(JNIEnv * env, jclass cls, jint pixel_width, jint pixel_height)
{
	UNUSED(env);
	UNUSED(cls);

	gameScreen->onSurfaceChanged(pixel_width, pixel_height);
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_on_1resume(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);

	gameScreen->onResume();
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_on_1pause(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);

	gameScreen->onPause();
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_update(JNIEnv* env, jclass cls, jfloat delta_time)
{
	UNUSED(env);
	UNUSED(cls);

	gameScreen->update(delta_time);
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_render(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);

	gameScreen->render();
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_on_1touch_1down(JNIEnv* env, jclass cls, jfloat raw_touch_x, jfloat raw_touch_y)
{
	UNUSED(env);
	UNUSED(cls);

	gameScreen->onTouch(DOWN, raw_touch_x, raw_touch_y);
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_on_1touch_1dragged(JNIEnv* env, jclass cls, jfloat raw_touch_x, jfloat raw_touch_y)
{
	UNUSED(env);
	UNUSED(cls);

	gameScreen->onTouch(DRAGGED, raw_touch_x, raw_touch_y);
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_on_1touch_1up(JNIEnv* env, jclass cls, jfloat raw_touch_x, jfloat raw_touch_y)
{
	UNUSED(env);
	UNUSED(cls);

	gameScreen->onTouch(UP, raw_touch_x, raw_touch_y);
}

JNIEXPORT short JNICALL Java_com_noctisgames_nosfuratu_Game_get_1current_1music_1id(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);

	return gameScreen->getCurrentMusicId();
}

JNIEXPORT short JNICALL Java_com_noctisgames_nosfuratu_Game_get_1current_1sound_1id(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);

	return gameScreen->getCurrentSoundId();
}

JNIEXPORT int JNICALL Java_com_noctisgames_nosfuratu_Game_get_1requested_1action(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);

	return gameScreen->getRequestedAction();
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_clear_1requested_1action(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);

	gameScreen->clearRequestedAction();
}

JNIEXPORT bool JNICALL Java_com_noctisgames_nosfuratu_Game_handle_1on_1back_1pressed(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);

	return gameScreen->handleOnBackPressed();
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_Game_load_1level(JNIEnv* env, jclass cls, jstring level_json)
{
	UNUSED(cls);

	const char *nativeLevelJson = (env)->GetStringUTFChars(level_json, nullptr);

	GameScreenLevelEditor::getInstance()->load(nativeLevelJson, gameScreen);
}

JNIEXPORT bool JNICALL Java_com_noctisgames_nosfuratu_Game_save_1level(JNIEnv *env, jclass cls, jstring json_file_path)
{
	UNUSED(env);
	UNUSED(cls);

	const char *level_json = GameScreenLevelEditor::getInstance()->save();
	const char *jsonFilePath = (env)->GetStringUTFChars(json_file_path, nullptr);
	if (level_json)
	{
		FILE *f = fopen(jsonFilePath, "w+, ccs=UTF-8");
		int sum = fprintf(f, "%s", level_json);
		fclose(f);

		return true;
	}
	else
	{
		return false;
	}
}