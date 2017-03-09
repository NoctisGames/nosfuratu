//
//  AndroidAudioEngineHelper.cpp
//  noctisgames-framework
//
//  Created by Stephen Gowen on 2/25/17.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "AndroidAudioEngineHelper.h"

#include "SoundWrapper.h"
#include "SuperpoweredSoundManager.h"
#include "SuperpoweredAndroidAudioIO.h"
#include "NGSTDUtil.h"

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_AndroidConfiguration.h>

#include <android/log.h>

#include <assert.h>
#include <sstream>

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

AndroidAudioEngineHelper* AndroidAudioEngineHelper::getInstance()
{
    static AndroidAudioEngineHelper *instance = new AndroidAudioEngineHelper();
    return instance;
}

void AndroidAudioEngineHelper::update()
{
    // Empty
}

void AndroidAudioEngineHelper::pause()
{
    // Empty
}

void AndroidAudioEngineHelper::resume()
{
    // Empty
}

SoundWrapper* AndroidAudioEngineHelper::loadSound(int soundId, const char *path, int numInstances)
{
    JNIEnv* env;
    
    jint status = m_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
    if (status != JNI_OK)
    {
        m_jvm->AttachCurrentThread(&env, NULL);
    }
    
    jclass class_resources = env->GetObjectClass(m_resources);
    
    jmethodID mid_getIdentifier = env->GetMethodID(class_resources, "getIdentifier", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I");
    assert(mid_getIdentifier != 0);
    
    jstring java_soundName = env->NewStringUTF(path);
    jstring java_defType = env->NewStringUTF("raw");
    
    jint resId = env->CallIntMethod(m_resources, mid_getIdentifier, java_soundName, java_defType, m_packageName);
    
    env->DeleteLocalRef(java_soundName);
    env->DeleteLocalRef(java_defType);
    
    jmethodID mid_openRawResourceFd = env->GetMethodID(class_resources, "openRawResourceFd", "(I)Landroid/content/res/AssetFileDescriptor;");
    assert(mid_openRawResourceFd != 0);
    
    jobject java_assetFileDescriptor = env->CallObjectMethod(m_resources, mid_openRawResourceFd, resId);
    
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
    
    if (status != JNI_OK)
    {
        m_jvm->DetachCurrentThread();
    }
    
    SuperpoweredSoundCollection* sound = new SuperpoweredSoundCollection(soundId, m_packageResourcePath, m_iSampleRate, numInstances, fileOffset, fileLength);
    
    return new SoundWrapper(sound);
}

SoundWrapper* AndroidAudioEngineHelper::loadMusic(const char* path)
{
    return loadSound(1337, path);
}

void AndroidAudioEngineHelper::init(JNIEnv* env, jobject activity)
{
    env->GetJavaVM(&m_jvm);
    
    jclass class_activity = env->GetObjectClass(activity);
    
    jmethodID mid_getPackageResourcePath = env->GetMethodID(class_activity, "getPackageResourcePath", "()Ljava/lang/String;");
    assert(mid_getPackageResourcePath != 0);
    m_javaPackageResourcePath = (jstring) env->CallObjectMethod(activity, mid_getPackageResourcePath);
    m_packageResourcePath = env->GetStringUTFChars(m_javaPackageResourcePath, JNI_FALSE);
    
    jmethodID mid_getPackageName = env->GetMethodID(class_activity, "getPackageName", "()Ljava/lang/String;");
    assert(mid_getPackageName != 0);
    jobject java_PackageName = env->CallObjectMethod(activity, mid_getPackageName);
    m_packageName = (jstring) env->NewGlobalRef(java_PackageName);
    
    jmethodID mid_getResources = env->GetMethodID(class_activity, "getResources", "()Landroid/content/res/Resources;");
    assert(mid_getResources != 0);
    jobject java_resources = env->CallObjectMethod(activity, mid_getResources);
    m_resources = env->NewGlobalRef(java_resources);
    
    // Get the device's sample rate and buffer size to enable low-latency Android audio output, if available.
    jmethodID mid_getSystemService = env->GetMethodID(class_activity, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
    assert(mid_getSystemService != 0);
    
    jstring java_audio = env->NewStringUTF("audio");
    jobject java_audioManager = env->CallObjectMethod(activity, mid_getSystemService, java_audio);
    
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
    
    m_iSampleRate = sampleRate;
    
    m_superpoweredSoundManager = new SuperpoweredSoundManager(m_iSampleRate, bufferSize);
    
    m_audioSystems.push_back(new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessingMusic, m_superpoweredSoundManager, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2));
    
    m_audioSystems.push_back(new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessingSound1, m_superpoweredSoundManager, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2));
    m_audioSystems.push_back(new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessingSound2, m_superpoweredSoundManager, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2));
    m_audioSystems.push_back(new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessingSound3, m_superpoweredSoundManager, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2));
    m_audioSystems.push_back(new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessingSound4, m_superpoweredSoundManager, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2));
    m_audioSystems.push_back(new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessingSound5, m_superpoweredSoundManager, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2));
    m_audioSystems.push_back(new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessingSound6, m_superpoweredSoundManager, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2));
    m_audioSystems.push_back(new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessingSound7, m_superpoweredSoundManager, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2));
    m_audioSystems.push_back(new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessingSound8, m_superpoweredSoundManager, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2));
    m_audioSystems.push_back(new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessingSound9, m_superpoweredSoundManager, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2));
    m_audioSystems.push_back(new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessingSound10, m_superpoweredSoundManager, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2));
    m_audioSystems.push_back(new SuperpoweredAndroidAudioIO(sampleRate, bufferSize, false, true, audioProcessingSound11, m_superpoweredSoundManager, -1, SL_ANDROID_STREAM_MEDIA, bufferSize * 2));
}

void AndroidAudioEngineHelper::deinit()
{
    NGSTDUtil::cleanUpVectorOfPointers(m_audioSystems);
    
    delete m_superpoweredSoundManager;
    m_superpoweredSoundManager = nullptr;
    
    JNIEnv* env;
    
    jint status = m_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
    if (status != JNI_OK)
    {
        m_jvm->AttachCurrentThread(&env, NULL);
    }
    
    env->DeleteLocalRef(m_resources);
    env->DeleteLocalRef(m_packageName);
    env->ReleaseStringUTFChars(m_javaPackageResourcePath, m_packageResourcePath);
    
    if (status != JNI_OK)
    {
        m_jvm->DetachCurrentThread();
    }
}

AndroidAudioEngineHelper::AndroidAudioEngineHelper() : IAudioEngineHelper(), m_superpoweredSoundManager(nullptr), m_iSampleRate(44100)
{
    // Empty
}
