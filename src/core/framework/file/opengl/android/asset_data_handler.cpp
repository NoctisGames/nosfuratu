//
//  asset_data_handler.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "asset_data_handler.h"

#include "AndroidAssetsManager.h"

FileData getAssetData(const char* relative_path)
{
    return ANDROID_ASSETS->getAssetData(relative_path);
}

void releaseAssetData(const FileData* file_data)
{
    ANDROID_ASSETS->releaseAssetData(file_data);
}
