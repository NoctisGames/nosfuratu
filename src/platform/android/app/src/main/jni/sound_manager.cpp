//
//  sound_manager.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 10/20/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include <jni.h>
#include <stdio.h>
#include "macros.h"
#include "SuperpoweredSoundManager.h"

SuperpoweredSoundManager *superpoweredSoundManager;

/* These functions are called from Java. */
extern "C"
{
JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_sound_SoundManager_init_1sound_1manager(JNIEnv* env, jclass cls, jstring apk_path, jint sample_rate, jint buffer_size);
    
JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_sound_SoundManager_load_1sound(JNIEnv* env, jclass cls, jint rawResourceId, jint fileOffset, jint fileLength);
    
JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_sound_SoundManager_play_1sound(JNIEnv* env, jclass cls, jint rawResourceId, jfloat volume, jboolean isLooping);
    
JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_sound_SoundManager_stop_1sound(JNIEnv* env, jclass cls, jint rawResourceId);
    
JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_sound_SoundManager_load_1music(JNIEnv* env, jclass cls, jint rawResourceId, jint fileOffset, jint fileLength);
    
JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_sound_SoundManager_play_1music(JNIEnv* env, jclass cls, jint rawResourceId, jfloat volume, jboolean isLooping);
    
JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_sound_SoundManager_set_1music_1volume(JNIEnv* env, jclass cls, jfloat volume);
    
JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_sound_SoundManager_resume_1music(JNIEnv* env, jclass cls);
    
JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_sound_SoundManager_pause_1music(JNIEnv* env, jclass cls);
};

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_sound_SoundManager_init_1sound_1manager(JNIEnv* env, jclass cls, jstring apk_path, jint sample_rate, jint buffer_size)
{
	UNUSED(cls);

    const char *path = env->GetStringUTFChars(apk_path, JNI_FALSE);
    
	superpoweredSoundManager = new SuperpoweredSoundManager(path, sample_rate, buffer_size);
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_sound_SoundManager_load_1sound(JNIEnv* env, jclass cls, jint rawResourceId, jint fileOffset, jint fileLength)
{
    UNUSED(env);
    UNUSED(cls);
    
    superpoweredSoundManager->loadSound(rawResourceId, fileOffset, fileLength);
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_sound_SoundManager_play_1sound(JNIEnv* env, jclass cls, jint rawResourceId, jfloat volume, jboolean isLooping)
{
    UNUSED(env);
    UNUSED(cls);
    
    superpoweredSoundManager->playSound(rawResourceId, volume, isLooping);
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_sound_SoundManager_stop_1sound(JNIEnv* env, jclass cls, jint rawResourceId)
{
    UNUSED(env);
    UNUSED(cls);
    
    superpoweredSoundManager->stopSound(rawResourceId);
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_sound_SoundManager_load_1music(JNIEnv* env, jclass cls, jint rawResourceId, jint fileOffset, jint fileLength)
{
    UNUSED(env);
    UNUSED(cls);
    
    superpoweredSoundManager->loadMusic(rawResourceId, fileOffset, fileLength);
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_sound_SoundManager_play_1music(JNIEnv* env, jclass cls, jint rawResourceId, jfloat volume, jboolean isLooping)
{
    UNUSED(env);
    UNUSED(cls);
    
    superpoweredSoundManager->playMusic(rawResourceId, volume, isLooping);
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_sound_SoundManager_set_1music_1volume(JNIEnv* env, jclass cls, jfloat volume)
{
    UNUSED(env);
    UNUSED(cls);
    
    superpoweredSoundManager->setMusicVolume(volume);
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_sound_SoundManager_resume_1music(JNIEnv* env, jclass cls)
{
    UNUSED(env);
    UNUSED(cls);
    
    superpoweredSoundManager->resumeMusic();
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_sound_SoundManager_pause_1music(JNIEnv* env, jclass cls)
{
    UNUSED(env);
    UNUSED(cls);
    
    superpoweredSoundManager->pauseMusic();
}
