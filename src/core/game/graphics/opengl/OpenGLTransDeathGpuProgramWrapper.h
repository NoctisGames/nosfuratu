//
//  OpenGLTransDeathGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 1/27/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__OpenGLTransDeathGpuProgramWrapper__
#define __nosfuratu__OpenGLTransDeathGpuProgramWrapper__

#include "TransDeathGpuProgramWrapper.h"

class OpenGLTransDeathProgram;

class OpenGLTransDeathGpuProgramWrapper : public TransDeathGpuProgramWrapper
{
public:
    OpenGLTransDeathGpuProgramWrapper(bool isTransIn);
    
    virtual ~OpenGLTransDeathGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
private:
    OpenGLTransDeathProgram* m_program;
};

#endif /* defined(__nosfuratu__OpenGLTransDeathGpuProgramWrapper__) */
