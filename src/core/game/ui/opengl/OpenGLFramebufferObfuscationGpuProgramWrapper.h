//
//  OpenGLFramebufferObfuscationGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/17/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__OpenGLFramebufferObfuscationGpuProgramWrapper__
#define __nosfuratu__OpenGLFramebufferObfuscationGpuProgramWrapper__

#include "GpuProgramWrapper.h"

#include "OpenGLFramebufferToScreenProgram.h"

class OpenGLFramebufferObfuscationGpuProgramWrapper : public GpuProgramWrapper
{
public:
    OpenGLFramebufferObfuscationGpuProgramWrapper();
    
    virtual ~OpenGLFramebufferObfuscationGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
private:
    OpenGLFramebufferToScreenProgramStruct m_program;
};

#endif /* defined(__nosfuratu__OpenGLFramebufferObfuscationGpuProgramWrapper__) */
