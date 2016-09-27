//
//  OpenGLESEndBossSnakeTextureGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/26/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__OpenGLESEndBossSnakeTextureGpuProgramWrapper__
#define __noctisgames__OpenGLESEndBossSnakeTextureGpuProgramWrapper__

#include "EndBossSnakeTextureGpuProgramWrapper.h"
#include "TextureProgram.h"

class OpenGLESEndBossSnakeTextureGpuProgramWrapper : public EndBossSnakeTextureGpuProgramWrapper
{
public:
    OpenGLESEndBossSnakeTextureGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
    virtual void cleanUp();
    
private:
    TextureProgramStruct m_program;
};

#endif /* defined(__noctisgames__OpenGLESEndBossSnakeTextureGpuProgramWrapper__) */
