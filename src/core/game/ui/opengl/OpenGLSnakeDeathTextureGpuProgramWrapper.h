//
//  OpenGLSnakeDeathTextureGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 12/30/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__OpenGLSnakeDeathTextureGpuProgramWrapper__
#define __nosfuratu__OpenGLSnakeDeathTextureGpuProgramWrapper__

#include "SnakeDeathTextureGpuProgramWrapper.h"

#include "OpenGLSnakeDeathTextureProgram.h"

class OpenGLSnakeDeathTextureGpuProgramWrapper : public SnakeDeathTextureGpuProgramWrapper
{
public:
    OpenGLSnakeDeathTextureGpuProgramWrapper();
    
    virtual ~OpenGLSnakeDeathTextureGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
private:
    OpenGLSnakeDeathTextureProgramStruct m_program;
};

#endif /* defined(__nosfuratu__OpenGLSnakeDeathTextureGpuProgramWrapper__) */
