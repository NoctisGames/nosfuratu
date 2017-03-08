//
//  AndroidAssetDataHandler.cpp
//  noctisgames-framework
//
//  Created by Stephen Gowen on 2/25/17.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "AndroidAssetDataHandler.h"

#include <assert.h>

AndroidAssetDataHandler* AndroidAssetDataHandler::getInstance()
{
    static AndroidAssetDataHandler *instance = new AndroidAssetDataHandler();
    return instance;
}

void AndroidAssetDataHandler::init(JNIEnv* env, jobject java_asset_manager)
{
    mAssetManager = AAssetManager_fromJava(env, java_asset_manager);
}

FileData AndroidAssetDataHandler::getAssetData(const char* relativePath)
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

void AndroidAssetDataHandler::releaseAssetData(const FileData* fileData)
{
    assert(fileData != NULL);
    assert(fileData->file_handle != NULL);
    
    AAsset_close((AAsset*)fileData->file_handle);
}

AndroidAssetDataHandler::AndroidAssetDataHandler() : AssetDataHandler()
{
    // Empty
}
