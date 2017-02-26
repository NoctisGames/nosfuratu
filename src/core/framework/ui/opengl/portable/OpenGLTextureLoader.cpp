//
//  OpenGLTextureLoader.cpp
//  noctisgames-framework
//
//  Created by Stephen Gowen on 1/15/17.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "OpenGLTextureLoader.h"

#include "GpuTextureDataWrapper.h"
#include "GpuTextureWrapper.h"
#include "AssetUtil.h"

#include <string>

OpenGLTextureLoader::OpenGLTextureLoader() : ITextureLoader()
{
    // Empty
}

GpuTextureDataWrapper* OpenGLTextureLoader::loadTextureData(const char* textureName)
{
    size_t len = strlen(textureName);
    
    char* textureFileName = new char[len + 5];
    
    strcpy(textureFileName, textureName);
    textureFileName[len] = '.';
    textureFileName[len+1] = 'p';
    textureFileName[len+2] = 'n';
    textureFileName[len+3] = 'g';
    textureFileName[len+4] = '\0';
    
    GpuTextureDataWrapper* tdw = new GpuTextureDataWrapper(AssetUtil::loadPngAsset(textureFileName));
    
    delete[] textureFileName;
    
    return tdw;
}

GpuTextureWrapper* OpenGLTextureLoader::loadTexture(GpuTextureDataWrapper* textureData, int repeatS)
{
    return new GpuTextureWrapper(AssetUtil::loadPngAssetIntoTexture(textureData->raw_image_data, repeatS));
}
