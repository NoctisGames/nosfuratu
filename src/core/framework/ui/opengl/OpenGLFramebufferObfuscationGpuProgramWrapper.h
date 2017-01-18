//
//  OpenGLFramebufferObfuscationGpuProgramWrapper.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 9/17/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__OpenGLFramebufferObfuscationGpuProgramWrapper__
#define __noctisgames__OpenGLFramebufferObfuscationGpuProgramWrapper__

#include "GpuProgramWrapper.h"
#include "FramebufferToScreenProgram.h"

class OpenGLFramebufferObfuscationGpuProgramWrapper : public GpuProgramWrapper
{
public:
    OpenGLFramebufferObfuscationGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
    virtual void cleanUp();
    
private:
    FramebufferToScreenProgramStruct m_program;
};

#endif /* defined(__noctisgames__OpenGLFramebufferObfuscationGpuProgramWrapper__) */
