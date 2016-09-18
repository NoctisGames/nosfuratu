//
//  OpenGLESFramebufferObfuscationGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/17/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__OpenGLESFramebufferObfuscationGpuProgramWrapper__
#define __noctisgames__OpenGLESFramebufferObfuscationGpuProgramWrapper__

#include "GpuProgramWrapper.h"
#include "FramebufferToScreenProgram.h"

class OpenGLESFramebufferObfuscationGpuProgramWrapper : public GpuProgramWrapper
{
public:
    OpenGLESFramebufferObfuscationGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
    virtual void cleanUp();
    
private:
    FramebufferToScreenProgramStruct m_program;
};

#endif /* defined(__noctisgames__OpenGLESFramebufferObfuscationGpuProgramWrapper__) */
