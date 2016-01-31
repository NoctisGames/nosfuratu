//
//  OpenGLESTransScreenGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 1/27/16.
//  Copyright (c) 2016 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__OpenGLESTransScreenGpuProgramWrapper__
#define __gowengamedev__OpenGLESTransScreenGpuProgramWrapper__

#include "TransitionGpuProgramWrapper.h"
#include "TransitionProgram.h"

class OpenGLESTransScreenGpuProgramWrapper : public TransitionGpuProgramWrapper
{
public:
    OpenGLESTransScreenGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
    virtual void cleanUp();
    
private:
    TransitionProgramStruct m_program;
};

#endif /* defined(__gowengamedev__OpenGLESTransScreenGpuProgramWrapper__) */
