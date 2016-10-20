//
//  OpenGLSnakeDeathTextureGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 12/30/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__OpenGLSnakeDeathTextureGpuProgramWrapper__
#define __noctisgames__OpenGLSnakeDeathTextureGpuProgramWrapper__

#include "SnakeDeathTextureGpuProgramWrapper.h"
#include "SnakeDeathTextureProgram.h"

class OpenGLSnakeDeathTextureGpuProgramWrapper : public SnakeDeathTextureGpuProgramWrapper
{
public:
    OpenGLSnakeDeathTextureGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
    virtual void cleanUp();
    
private:
    SnakeDeathTextureProgramStruct m_program;
};

#endif /* defined(__noctisgames__OpenGLSnakeDeathTextureGpuProgramWrapper__) */
