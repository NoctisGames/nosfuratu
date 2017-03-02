//
//  AndroidAssetsManager.cpp
//  noctisgames-framework
//
//  Created by Stephen Gowen on 2/25/17.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "AndroidAssetsManager.h"

#include <assert.h>

AndroidAssetsManager* AndroidAssetsManager::getInstance()
{
    static AndroidAssetsManager *instance = new AndroidAssetsManager();
    return instance;
}

void AndroidAssetsManager::init(JNIEnv* env, jobject java_asset_manager)
{
    mAssetManager = AAssetManager_fromJava(env, java_asset_manager);
}

FileData AndroidAssetsManager::getAssetData(const char* relativePath)
{
    assert(relativePath != NULL);
    
    AAsset *asset = AAssetManager_open(mAssetManager, relativePath, AASSET_MODE_STREAMING);
    assert(asset != NULL);
    
    return (FileData)
    {
        AAsset_getLength(asset),
        AAsset_getBuffer(asset),
        asset
    };
}

void AndroidAssetsManager::releaseAssetData(const FileData* fileData)
{
    assert(fileData != NULL);
    assert(fileData->file_handle != NULL);
    
    AAsset_close((AAsset*)fileData->file_handle);
}
