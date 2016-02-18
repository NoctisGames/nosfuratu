//
//  OpenGLESFramebufferTintGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 1/23/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__OpenGLESFramebufferTintGpuProgramWrapper__
#define __noctisgames__OpenGLESFramebufferTintGpuProgramWrapper__

#include "GpuProgramWrapper.h"
#include "FramebufferToScreenProgram.h"

class OpenGLESFramebufferTintGpuProgramWrapper : public GpuProgramWrapper
{
public:
    OpenGLESFramebufferTintGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
    virtual void cleanUp();
    
private:
    FramebufferToScreenProgramStruct m_program;
};

#endif /* defined(__noctisgames__OpenGLESFramebufferTintGpuProgramWrapper__) */
