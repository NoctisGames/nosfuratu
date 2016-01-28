//
//  OpenGLESFrameBufferTintGpuProgramWrapper.h
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 1/23/16.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__OpenGLESFrameBufferTintGpuProgramWrapper__
#define __gowengamedev__OpenGLESFrameBufferTintGpuProgramWrapper__

#include "GpuProgramWrapper.h"
#include "FrameBufferToScreenProgram.h"

class OpenGLESFrameBufferTintGpuProgramWrapper : public GpuProgramWrapper
{
public:
    OpenGLESFrameBufferTintGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
    virtual void cleanUp();
    
private:
    FrameBufferToScreenProgramStruct m_program;
};

#endif /* defined(__gowengamedev__OpenGLESFrameBufferTintGpuProgramWrapper__) */
