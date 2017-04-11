//
//  OpenGLEndBossSnakeTextureGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/26/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__OpenGLEndBossSnakeTextureGpuProgramWrapper__
#define __nosfuratu__OpenGLEndBossSnakeTextureGpuProgramWrapper__

#include "EndBossSnakeTextureGpuProgramWrapper.h"

class OpenGLTextureProgram;

class OpenGLEndBossSnakeTextureGpuProgramWrapper : public EndBossSnakeTextureGpuProgramWrapper
{
public:
    OpenGLEndBossSnakeTextureGpuProgramWrapper();
    
    virtual ~OpenGLEndBossSnakeTextureGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
private:
    OpenGLTextureProgram* m_program;
};

#endif /* defined(__nosfuratu__OpenGLEndBossSnakeTextureGpuProgramWrapper__) */
