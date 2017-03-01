//
//  game.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "macros.h"
#include "MainScreen.h"
#include "MainScreenWorldMap.h"
#include "ScreenInputManager.h"
#include "KeyboardInputManager.h"
#include "SoundManager.h"
#include "MainAssets.h"
#include "MainScreenTitle.h"
#include "AndroidAssetsManager.h"
#include "SuperpoweredSoundManager.h"
#include "SuperpoweredAndroidAudioIO.h"
#include "GameConstants.h"
#include "VectorUtil.h"

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_AndroidConfiguration.h>

#include <jni.h>
#include <android/log.h>

#include <assert.h>
#include <stdio.h>
#include <sstream>

jobject gActivity;
jobject gResources;

jstring gJavaPackageResourcePath;
const char* gPackageResourcePath;
jstring gPackageName;

MainScreen* gScreen;

SuperpoweredSoundManager* gSuperpoweredSoundManager;
std::vector<SuperpoweredAndroidAudioIO*> gAudioSystems;

int gSoundIndexCounter;

static bool audioProcessingMusic(void *clientData, short int *audioIO, int numberOfSamples, int __unused samplerate)
{
    return ((SuperpoweredSoundManager *)clientData)->processMusic(audioIO, (unsigned int)numberOfSamples);
}

static bool audioProcessingSound1(void *clientData, short int *audioIO, int numberOfSamples, int __unused samplerate)
{
    return ((SuperpoweredSoundManager *)clientData)->processSound1(audioIO, (unsigned int)numberOfSamples);
}

static bool audioProcessingSound2(void *clientData, short int *audioIO, int numberOfSamples, int __unused samplerate)
{
    return ((SuperpoweredSoundManager *)clientData)->processSound2(audioIO, (unsigned int)numberOfSamples);
}

static bool audioProcessingSound3(void *clientData, short int *audioIO, int numberOfSamples, int __unused samplerate)
{
    return ((SuperpoweredSoundManager *)clientData)->processSound3(audioIO, (unsigned int)numberOfSamples);
}

static bool audioProcessingSound4(void *clientData, short int *audioIO, int numberOfSamples, int __unused samplerate)
{
    return ((SuperpoweredSoundManager *)clientData)->processSound4(audioIO, (unsigned int)numberOfSamples);
}

static bool audioProcessingSound5(void *clientData, short int *audioIO, int numberOfSamples, int __unused samplerate)
{
    return ((SuperpoweredSoundManager *)clientData)->processSound5(audioIO, (unsigned int)numberOfSamples);
}

static bool audioProcessingSound6(void *clientData, short int *audioIO, int numberOfSamples, int __unused samplerate)
{
    return ((SuperpoweredSoundManager *)clientData)->processSound6(audioIO, (unsigned int)numberOfSamples);
}

static bool audioProcessingSound7(void *clientData, short int *audioIO, int numberOfSamples, int __unused samplerate)
{
    return ((SuperpoweredSoundManager *)clientData)->processSound7(audioIO, (unsigned int)numberOfSamples);
}

static bool audioProcessingSound8(void *clientData, short int *audioIO, int numberOfSamples, int __unused samplerate)
{
    return ((SuperpoweredSoundManager *)clientData)->processSound8(audioIO, (unsigned int)numberOfSamples);
}

static bool audioProcessingSound9(void *clientData, short int *audioIO, int numberOfSamples, int __unused samplerate)
{
    return ((SuperpoweredSoundManager *)clientData)->processSound9(audioIO, (unsigned int)numberOfSamples);
}

static bool audioProcessingSound10(void *clientData, short int *audioIO, int numberOfSamples, int __unused samplerate)
{
    return ((SuperpoweredSoundManager *)clientData)->processSound10(audioIO, (unsigned int)numberOfSamples);
}

static bool audioProcessingSound11(void *clientData, short int *audioIO, int numberOfSamples, int __unused samplerate)
{
    return ((SuperpoweredSoundManager *)clientData)->processSound11(audioIO, (unsigned int)numberOfSamples);
}

void loadSound(JNIEnv* env, const char* soundName, int numCopies, int rawResourceId = -1)
{
    jclass class_resources = env->GetObjectClass(gResources);
    
    jmethodID mid_getIdentifier = env->GetMethodID(class_resources, "getIdentifier", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I");
    assert(mid_getIdentifier != 0);
    
    jstring java_soundName = env->NewStringUTF(soundName);
    jstring java_defType = env->NewStringUTF("raw");
    
    jint resId = env->CallIntMethod(gResources, mid_getIdentifier, java_soundName, java_defType, gPackageName);
    
    env->DeleteLocalRef(java_soundName);
    env->DeleteLocalRef(java_defType);
    
    jmethodID mid_openRawResourceFd = env->GetMethodID(class_resources, "openRawResourceFd", "(I)Landroid/content/res/AssetFileDescriptor;");
    assert(mid_openRawResourceFd != 0);
    
    jobject java_assetFileDescriptor = env->CallObjectMethod(gResources, mid_openRawResourceFd, resId);
    
    jclass class_assetFileDescriptor = env->GetObjectClass(java_assetFileDescriptor);
    
    jmethodID mid_getStartOffset = env->GetMethodID(class_assetFileDescriptor, "getStartOffset", "()J");
    assert(mid_getStartOffset != 0);
    
    jlong java_fileOffset = env->CallLongMethod(java_assetFileDescriptor, mid_getStartOffset);
    
    jmethodID mid_getLength = env->GetMethodID(class_assetFileDescriptor, "getLength", "()J");
    assert(mid_getLength != 0);
    
    jlong java_fileLength = env->CallLongMethod(java_assetFileDescriptor, mid_getLength);
    
    int fileOffset = (int) java_fileOffset;
    int fileLength = (int) java_fileLength;
    
    jmethodID mid_getParcelFileDescriptor = env->GetMethodID(class_assetFileDescriptor, "getParcelFileDescriptor", "()Landroid/os/ParcelFileDescriptor;");
    assert(mid_getParcelFileDescriptor != 0);
    
    jobject java_parcelFileDescriptor = env->CallObjectMethod(java_assetFileDescriptor, mid_getParcelFileDescriptor);
    
    jclass class_parcelFileDescriptor = env->GetObjectClass(java_parcelFileDescriptor);
    jmethodID mid_close = env->GetMethodID(class_parcelFileDescriptor, "close", "()V");
    assert(mid_close != 0);
    
    env->CallVoidMethod(java_parcelFileDescriptor, mid_close);
    
    env->DeleteLocalRef(java_assetFileDescriptor);
    env->DeleteLocalRef(java_parcelFileDescriptor);
    
    if (rawResourceId == -1)
    {
        gSuperpoweredSoundManager->loadSound(gSoundIndexCounter++, gPackageResourcePath, numCopies, fileOffset, fileLength);
    }
    else
    {
        gSuperpoweredSoundManager->loadMusic(rawResourceId, gPackageResourcePath, fileOffset, fileLength);
    }
}

void loadMusic(JNIEnv* env, const char* musicName)
{
    loadSound(env, musicName, 1, 1337);
}

void handleSound()
{
    short soundId;
    while ((soundId = SOUND_MANAGER->getCurrentSoundId()) > 0)
    {
        switch (soundId)
        {
            case SOUND_JON_VAMPIRE_GLIDE:
            case SOUND_SPARROW_FLY:
            case SOUND_SAW_GRIND:
            case SOUND_SPIKED_BALL_ROLLING:
                gSuperpoweredSoundManager->playSound((soundId - 1), 1, true);
                break;
            case STOP_SOUND_JON_VAMPIRE_GLIDE:
            case STOP_SOUND_SPARROW_FLY:
            case STOP_SOUND_SAW_GRIND:
            case STOP_SOUND_SPIKED_BALL_ROLLING:
                gSuperpoweredSoundManager->stopSound((soundId - 1000 - 1));
                break;
            case RESUME_ALL_SOUNDS:
                gSuperpoweredSoundManager->resumeAllSounds();
                break;
            case PAUSE_ALL_SOUNDS:
                gSuperpoweredSoundManager->pauseAllSounds();
                break;
            case STOP_ALL_SOUNDS:
                gSuperpoweredSoundManager->stopAllSounds();
                break;
            case STOP_ALL_LOOPING_SOUNDS:
                gSuperpoweredSoundManager->stopAllLoopingSounds();
                break;
            default:
                gSuperpoweredSoundManager->playSound((soundId - 1), 1, false);
                break;
        }
    }
}

void handleMusic(JNIEnv* env)
{
    short rawMusicId;
    while ((rawMusicId = SOUND_MANAGER->getCurrentMusicId()) > 0)
    {
        short musicId = rawMusicId;
        if (musicId >= 1000)
        {
            musicId /= 1000;
            rawMusicId -= musicId * 1000;
        }
        
        switch (musicId)
        {
            case MUSIC_STOP:
                gSuperpoweredSoundManager->pauseMusic();
                break;
            case MUSIC_RESUME:
                gSuperpoweredSoundManager->resumeMusic();
                break;
            case MUSIC_PLAY:
                gSuperpoweredSoundManager->playMusic(1.0f, false);
                break;
            case MUSIC_PLAY_LOOP:
                gSuperpoweredSoundManager->playMusic(1.0f, true);
                break;
            case MUSIC_SET_VOLUME:
            {
                float volume = rawMusicId / 100.0f;
                
                gSuperpoweredSoundManager->setMusicVolume(volume);
            }
                break;
            case MUSIC_LOAD_TITLE_LOOP:
                loadMusic(env, "title_bgm");
                break;
            case MUSIC_LOAD_OPENING_CUTSCENE:
                loadMusic(env, "opening_cutscene_bgm");
                break;
            case MUSIC_LOAD_LEVEL_SELECT_LOOP:
                loadMusic(env, "level_select_bgm");
                break;
            case MUSIC_LOAD_WORLD_1_LOOP:
                loadMusic(env, "world_1_bgm");
                break;
            case MUSIC_LOAD_MID_BOSS_LOOP:
                loadMusic(env, "mid_boss_bgm");
                break;
            case MUSIC_LOAD_END_BOSS_LOOP:
                loadMusic(env, "final_boss_bgm");
                break;
            default:
                break;
        }
    }
}

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
    
    JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_load_1user_1save_1data(JNIEnv *env, jclass cls, jstring json);
    
    JNIEXPORT int JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_get_1score(JNIEnv* env, jclass cls);
    
    JNIEXPORT int JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_get_1online_1score(JNIEnv* env, jclass cls);
    
    JNIEXPORT int JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_get_1level_1stats_1flag(JNIEnv* env, jclass cls);
    
    JNIEXPORT int JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_get_1num_1golden_1carrots(JNIEnv* env, jclass cls);
    
    JNIEXPORT int JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_get_1jon_1unlocked_1abilities_1flag(JNIEnv* env, jclass cls);
    
    JNIEXPORT int JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_get_1level_1stats_1flag_1for_1unlocked_1level(JNIEnv* env, jclass cls);
    
    JNIEXPORT int JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_get_1num_1golden_1carrots_1after_1unlocking_1level(JNIEnv* env, jclass cls);
};

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_init(JNIEnv* env, jclass cls, jobject activity, jboolean isLowMemoryDevice)
{
    UNUSED(cls);
    
    gActivity = env->NewGlobalRef(activity);
    
    jclass class_activity = env->GetObjectClass(gActivity);
    jmethodID mid_getAssets = env->GetMethodID(class_activity, "getAssets", "()Landroid/content/res/AssetManager;");
    assert(mid_getAssets != 0);
    
    jobject java_AssetManager = env->CallObjectMethod(gActivity, mid_getAssets);
    
    ANDROID_ASSETS->init(env, java_AssetManager);
    
    jmethodID mid_getPackageResourcePath = env->GetMethodID(class_activity, "getPackageResourcePath", "()Ljava/lang/String;");
    assert(mid_getPackageResourcePath != 0);
    gJavaPackageResourcePath = (jstring) env->CallObjectMethod(gActivity, mid_getPackageResourcePath);
    gPackageResourcePath = env->GetStringUTFChars(gJavaPackageResourcePath, JNI_FALSE);
    
    jmethodID mid_getPackageName = env->GetMethodID(class_activity, "getPackageName", "()Ljava/lang/String;");
    assert(mid_getPackageName != 0);
    jobject java_PackageName = env->CallObjectMethod(gActivity, mid_getPackageName);
    gPackageName = (jstring) env->NewGlobalRef(java_PackageName);
    
    jmethodID mid_getResources = env->GetMethodID(class_activity, "getResources", "()Landroid/content/res/Resources;");
    assert(mid_getResources != 0);
    jobject java_resources = env->CallObjectMethod(gActivity, mid_getResources);
    gResources = env->NewGlobalRef(java_resources);
    
    // Get the device's sample rate and buffer size to enable low-latency Android audio output, if available.
    jmethodID mid_getSystemService = env->GetMethodID(class_activity, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
    assert(mid_getSystemService != 0);
    
    jstring java_audio = env->NewStringUTF("audio");
    jobject java_audioManager = env->CallObjectMethod(gActivity, mid_getSystemService, java_audio);
    
    env->DeleteLocalRef(java_audio);
    
    int sampleRate;
    int bufferSize;
    
    jclass class_audioManager = env->GetObjectClass(java_audioManager);
    
    {
        jmethodID mid_getProperty = env->GetMethodID(class_audioManager, "getProperty", "(Ljava/lang/String;)Ljava/lang/String;");
        assert(mid_getProperty != 0);
        
        jstring java_property = env->NewStringUTF("android.media.property.OUTPUT_SAMPLE_RATE");
        jstring java_sampleRateString = (jstring) env->CallObjectMethod(java_audioManager, mid_getProperty, java_property);
        if (java_sampleRateString == NULL)
        {
            sampleRate = 44100;
        }
        else
        {
            const char *sampleRateString = env->GetStringUTFChars(java_sampleRateString, JNI_FALSE);
            
            std::stringstream ss;
            ss << sampleRateString;
            
            ss >> sampleRate;
        }
        
        env->DeleteLocalRef(java_property);
        env->DeleteLocalRef(java_sampleRateString);
    }
    
    {
        jmethodID mid_getProperty = env->GetMethodID(class_audioManager, "getProperty", "(Ljava/lang/String;)Ljava/lang/String;");
        assert(mid_getProperty != 0);
        
        jstring java_property = env->NewStringUTF("android.media.property.OUTPUT_FRAMES_PER_BUFFER");
        jstring java_bufferSizeString = (jstring) env->CallObjectMethod(java_audioManager, mid_getProperty, java_property);
        if (java_bufferSizeString == NULL)
        {
            bufferSize = 512;
        }
        else
        {
            const char *bufferSizeString = env->GetStringUTFChars(java_bufferSizeString, JNI_FALSE);
            
            std::stringstream ss;
            ss << bufferSizeString;
            
            ss >> bufferSize;
        }
        
        env->DeleteLocalRef(java_property);
        env->DeleteLocalRef(java_bufferSizeString);
    }
    
    env->DeleteLocalRef(java_audioManager);
    
    __android_log_print(ANDROID_LOG_VERBOSE, "NosFURatu", "sampleRate %d", sampleRate);
    __android_log_print(ANDROID_LOG_VERBOSE, "NosFURatu", "bufferSize %d", bufferSize);
    
    gSuperpoweredSoundManager = new SuperpoweredSoundManager(sampleRate, bufferSize);
    
    gAudioSystems.push_back(new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessingMusic, gSuperpoweredSoundManager, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2));
    
    gAudioSystems.push_back(new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessingSound1, gSuperpoweredSoundManager, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2));
    gAudioSystems.push_back(new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessingSound2, gSuperpoweredSoundManager, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2));
    gAudioSystems.push_back(new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessingSound3, gSuperpoweredSoundManager, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2));
    gAudioSystems.push_back(new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessingSound4, gSuperpoweredSoundManager, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2));
    gAudioSystems.push_back(new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessingSound5, gSuperpoweredSoundManager, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2));
    gAudioSystems.push_back(new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessingSound6, gSuperpoweredSoundManager, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2));
    gAudioSystems.push_back(new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessingSound7, gSuperpoweredSoundManager, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2));
    gAudioSystems.push_back(new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessingSound8, gSuperpoweredSoundManager, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2));
    gAudioSystems.push_back(new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessingSound9, gSuperpoweredSoundManager, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2));
    gAudioSystems.push_back(new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessingSound10, gSuperpoweredSoundManager, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2));
    gAudioSystems.push_back(new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessingSound11, gSuperpoweredSoundManager, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2));
    
    gSoundIndexCounter = 0;
    
    loadSound(env, "collect_carrot", 6);
    loadSound(env, "collect_golden_carrot", 1);
    loadSound(env, "death", 1);
    loadSound(env, "footstep_left_grass", 1);
    loadSound(env, "footstep_right_grass", 1);
    loadSound(env, "footstep_left_cave", 1);
    loadSound(env, "footstep_right_cave", 1);
    loadSound(env, "jump_spring", 1);
    loadSound(env, "landing_grass", 1);
    loadSound(env, "landing_cave", 1);
    loadSound(env, "snake_death", 2);
    loadSound(env, "trigger_transform", 1);
    loadSound(env, "cancel_transform", 1);
    loadSound(env, "complete_transform", 1);
    loadSound(env, "jump_spring_heavy", 1);
    loadSound(env, "jon_rabbit_jump", 1);
    loadSound(env, "jon_vampire_jump", 1);
    loadSound(env, "jon_rabbit_double_jump", 1);
    loadSound(env, "jon_vampire_double_jump", 1);
    loadSound(env, "vampire_glide_loop", 1);
    loadSound(env, "mushroom_bounce", 1);
    loadSound(env, "jon_burrow_rocksfall", 1);
    loadSound(env, "sparrow_fly_loop", 4);
    loadSound(env, "sparrow_die", 2);
    loadSound(env, "toad_die", 1);
    loadSound(env, "toad_eat", 1);
    loadSound(env, "saw_grind_loop", 4);
    loadSound(env, "fox_bounced_on", 1);
    loadSound(env, "fox_strike", 4);
    loadSound(env, "fox_death", 2);
    loadSound(env, "world_1_bgm_intro", 1);
    loadSound(env, "mid_boss_bgm_intro", 1);
    loadSound(env, "mid_boss_owl_swoop", 1);
    loadSound(env, "mid_boss_owl_tree_smash", 1);
    loadSound(env, "mid_boss_owl_death", 1);
    loadSound(env, "screen_transition", 1);
    loadSound(env, "screen_transition_2", 1);
    loadSound(env, "level_complete", 1);
    loadSound(env, "title_lightning_1", 2);
    loadSound(env, "title_lightning_2", 2);
    loadSound(env, "ability_unlock", 1);
    loadSound(env, "boss_level_clear", 1);
    loadSound(env, "level_clear", 1);
    loadSound(env, "level_selected", 2);
    loadSound(env, "rabbit_drill", 1);
    loadSound(env, "snake_jump", 2);
    loadSound(env, "vampire_dash", 1);
    loadSound(env, "boss_level_unlock", 1);
    loadSound(env, "rabbit_stomp", 1);
    loadSound(env, "final_boss_bgm_intro", 1);
    loadSound(env, "button_click", 1);
    loadSound(env, "level_confirmed", 1);
    loadSound(env, "bat_poof", 1);
    loadSound(env, "chain_snap", 1);
    loadSound(env, "end_boss_snake_mouth_open", 1);
    loadSound(env, "end_boss_snake_charge_cue", 1);
    loadSound(env, "end_boss_snake_charge", 1);
    loadSound(env, "end_boss_snake_damaged", 1);
    loadSound(env, "end_boss_snake_death", 1);
    loadSound(env, "spiked_ball_rolling_loop", 2);
    loadSound(env, "absorb_dash_ability", 1);
    loadSound(env, "footstep_left_wood", 1);
    loadSound(env, "footstep_right_wood", 1);
    loadSound(env, "landing_wood", 1);
    loadSound(env, "collect_big_carrot", 1);
    loadSound(env, "collect_vial", 1);
    
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
    
    gSuperpoweredSoundManager->pauseMusic();
    
    gSuperpoweredSoundManager->stopSound(SOUND_JON_VAMPIRE_GLIDE - 1);
    gSuperpoweredSoundManager->stopSound(SOUND_SPARROW_FLY - 1);
    gSuperpoweredSoundManager->stopSound(SOUND_SAW_GRIND - 1);
    gSuperpoweredSoundManager->stopSound(SOUND_SPIKED_BALL_ROLLING - 1);
}

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_on_1stop(JNIEnv* env, jclass cls)
{
    UNUSED(env);
    UNUSED(cls);
    
    VectorUtil::cleanUpVectorOfPointers(gAudioSystems);
    
    delete gSuperpoweredSoundManager;
    gSuperpoweredSoundManager = nullptr;
    
    delete gScreen;
    gScreen = nullptr;
    
    env->DeleteLocalRef(gActivity);
    env->DeleteLocalRef(gResources);
    env->DeleteLocalRef(gPackageName);
    env->ReleaseStringUTFChars(gJavaPackageResourcePath, gPackageResourcePath);
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
    
    handleSound();
    handleMusic(env);
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

JNIEXPORT void JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_load_1user_1save_1data(JNIEnv *env, jclass cls, jstring json)
{
	UNUSED(env);
	UNUSED(cls);

	const char *nativeJson = env->GetStringUTFChars(json, nullptr);

	WorldMap::getInstance()->loadUserSaveData(nativeJson);
    
    env->ReleaseStringUTFChars(json, nativeJson);
}

JNIEXPORT int JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_get_1score(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);
    
	return gScreen->getScore();
}

JNIEXPORT int JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_get_1online_1score(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);
    
	return gScreen->getOnlineScore();
}

JNIEXPORT int JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_get_1level_1stats_1flag(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);
    
	return gScreen->getLevelStatsFlag();
}

JNIEXPORT int JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_get_1num_1golden_1carrots(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);
    
	return gScreen->getNumGoldenCarrots();
}

JNIEXPORT int JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_get_1jon_1unlocked_1abilities_1flag(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);
    
	return gScreen->getJonAbilityFlag();
}

JNIEXPORT int JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_get_1level_1stats_1flag_1for_1unlocked_1level(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);
    
	return gScreen->getLevelStatsFlagForUnlockedLevel();
}

JNIEXPORT int JNICALL Java_com_noctisgames_nosfuratu_AndroidMain_get_1num_1golden_1carrots_1after_1unlocking_1level(JNIEnv* env, jclass cls)
{
	UNUSED(env);
	UNUSED(cls);
    
	return gScreen->getNumGoldenCarrotsAfterUnlockingLevel();
}
