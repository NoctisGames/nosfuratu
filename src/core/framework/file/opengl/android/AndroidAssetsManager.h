//
//  AndroidAssetsManager.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 2/25/17.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__AndroidAssetsManager__
#define __noctisgames__AndroidAssetsManager__

#define ANDROID_ASSETS (AndroidAssetsManager::getInstance())

#include "FileData.h"

#include <android/asset_manager_jni.h>

class AndroidAssetsManager
{
public:
	static AndroidAssetsManager* getInstance();
    
    void init(JNIEnv* env, jobject java_asset_manager);
    
    FileData getAssetData(const char* relativePath);
    
    void releaseAssetData(const FileData* fileData);

private:
    AAssetManager* mAssetManager;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    AndroidAssetsManager() {}
    AndroidAssetsManager(const AndroidAssetsManager&);
    AndroidAssetsManager& operator=(const AndroidAssetsManager&);
};

#endif /* defined(__noctisgames__AndroidAssetsManager__) */
