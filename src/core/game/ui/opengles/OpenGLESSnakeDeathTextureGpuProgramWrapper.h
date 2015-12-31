//
//  OpenGLESSnakeDeathTextureGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 12/30/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__OpenGLESSnakeDeathTextureGpuProgramWrapper__
#define __gowengamedev__OpenGLESSnakeDeathTextureGpuProgramWrapper__

#include "SnakeDeathTextureGpuProgramWrapper.h"
#include "SnakeDeathTextureProgram.h"

class OpenGLESSnakeDeathTextureGpuProgramWrapper : public SnakeDeathTextureGpuProgramWrapper
{
public:
    OpenGLESSnakeDeathTextureGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
    virtual void cleanUp();
    
private:
    SnakeDeathTextureProgramStruct m_program;
};

#endif /* defined(__gowengamedev__OpenGLESSnakeDeathTextureGpuProgramWrapper__) */
