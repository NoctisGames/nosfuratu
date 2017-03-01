//
//  asset_data_handler.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "asset_data_handler.h"

#include "AndroidAssetsManager.h"

FileData get_asset_data(const char* relative_path)
{
    return ANDROID_ASSETS->getAssetData(relative_path);
}

void release_asset_data(const FileData* file_data)
{
    ANDROID_ASSETS->releaseAssetData(file_data);
}
