//
//  OpenGLEndBossSnakeTextureGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/26/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__OpenGLEndBossSnakeTextureGpuProgramWrapper__
#define __noctisgames__OpenGLEndBossSnakeTextureGpuProgramWrapper__

#include "EndBossSnakeTextureGpuProgramWrapper.h"

#include "TextureProgram.h"

class OpenGLEndBossSnakeTextureGpuProgramWrapper : public EndBossSnakeTextureGpuProgramWrapper
{
public:
    OpenGLEndBossSnakeTextureGpuProgramWrapper();
    
    virtual ~OpenGLEndBossSnakeTextureGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
private:
    TextureProgramStruct m_program;
};

#endif /* defined(__noctisgames__OpenGLEndBossSnakeTextureGpuProgramWrapper__) */
