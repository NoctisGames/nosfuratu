//
//  OpenGLESFramebufferRadialBlurGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 1/29/16.
//  Copyright (c) 2016 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__OpenGLESFramebufferRadialBlurGpuProgramWrapper__
#define __gowengamedev__OpenGLESFramebufferRadialBlurGpuProgramWrapper__

#include "GpuProgramWrapper.h"
#include "FramebufferToScreenProgram.h"

class OpenGLESFramebufferRadialBlurGpuProgramWrapper : public GpuProgramWrapper
{
public:
    OpenGLESFramebufferRadialBlurGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
    virtual void cleanUp();
    
private:
    FramebufferToScreenProgramStruct m_program;
};

#endif /* defined(__gowengamedev__OpenGLESFramebufferRadialBlurGpuProgramWrapper__) */
