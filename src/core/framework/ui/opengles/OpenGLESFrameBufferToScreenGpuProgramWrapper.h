//
//  OpenGLESFrameBufferToScreenGpuProgramWrapper.h
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 9/3/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__OpenGLESFrameBufferToScreenGpuProgramWrapper__
#define __nosfuratu__OpenGLESFrameBufferToScreenGpuProgramWrapper__

#include "GpuProgramWrapper.h"
#include "FrameBufferToScreenProgram.h"

class OpenGLESFrameBufferToScreenGpuProgramWrapper : public GpuProgramWrapper
{
public:
    OpenGLESFrameBufferToScreenGpuProgramWrapper(FrameBufferToScreenProgramStruct program);
    
    virtual void bind();
    
    virtual void unbind();
    
private:
    FrameBufferToScreenProgramStruct m_program;
};

#endif /* defined(__nosfuratu__OpenGLESFrameBufferToScreenGpuProgramWrapper__) */
