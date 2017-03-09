//
//  AndroidSuperpoweredAudioEngineHelper.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 2/25/17.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__AndroidSuperpoweredAudioEngineHelper__
#define __noctisgames__AndroidSuperpoweredAudioEngineHelper__

#define ANDROID_SUPERPOWERED_AUDIO_ENGINE_HELPER (AndroidSuperpoweredAudioEngineHelper::getInstance())

#include "NGSuperpoweredAudioEngineHelper.h"

#include <android/asset_manager_jni.h>

class AndroidSuperpoweredAudioEngineHelper : public NGSuperpoweredAudioEngineHelper
{
public:
	static AndroidSuperpoweredAudioEngineHelper* getInstance();
    
    virtual void loadSound(int soundId, const char* soundName, int numCopies);
    
    virtual void loadMusic(const char* musicName);
    
    void init(JNIEnv* env, jobject activity);
    
    void deinit();

private:
    JavaVM* m_jvm;
    jobject m_resources;
    jstring m_javaPackageResourcePath;
    const char* m_packageResourcePath;
    jstring m_packageName;
    NGSuperpoweredSoundManager* m_superpoweredSoundManager;
    std::vector<SuperpoweredAndroidAudioIO*> m_audioSystems;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    AndroidSuperpoweredAudioEngineHelper();
    AndroidSuperpoweredAudioEngineHelper(const AndroidSuperpoweredAudioEngineHelper&);
    AndroidSuperpoweredAudioEngineHelper& operator=(const AndroidSuperpoweredAudioEngineHelper&);
};

#endif /* defined(__noctisgames__AndroidSuperpoweredAudioEngineHelper__) */
