//
//  AssetUtil.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__AssetUtil__
#define __noctisgames__AssetUtil__

extern "C"
{
#include "platform_gl.h"
#include "image.h"
}

class AssetUtil
{
public:
    static RawImageData loadPngAsset(const char* relative_path);
    
    static GLuint loadPngAssetIntoTexture(RawImageData raw_image_data, int repeat_s);
    
    static GLuint buildProgramFromAssets(const char* vertex_shader_path, const char* fragment_shader_path);
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    AssetUtil() {}
    AssetUtil(const AssetUtil&);
    AssetUtil& operator=(const AssetUtil&);
};

#endif /* defined(__noctisgames__AssetUtil__) */
