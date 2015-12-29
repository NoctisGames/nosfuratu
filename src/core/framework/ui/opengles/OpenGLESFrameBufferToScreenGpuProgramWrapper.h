//
//  OpenGLESFrameBufferToScreenGpuProgramWrapper.h
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 9/3/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__OpenGLESFrameBufferToScreenGpuProgramWrapper__
#define __gowengamedev__OpenGLESFrameBufferToScreenGpuProgramWrapper__

#include "GpuProgramWrapper.h"
#include "FrameBufferToScreenProgram.h"

class OpenGLESFrameBufferToScreenGpuProgramWrapper : public GpuProgramWrapper
{
public:
    OpenGLESFrameBufferToScreenGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
    virtual void cleanUp();
    
private:
    FrameBufferToScreenProgramStruct m_program;
};

#endif /* defined(__gowengamedev__OpenGLESFrameBufferToScreenGpuProgramWrapper__) */
