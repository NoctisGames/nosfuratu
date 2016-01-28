//
//  OpenGLESTransDeathGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 1/27/16.
//  Copyright (c) 2016 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__OpenGLESTransDeathGpuProgramWrapper__
#define __gowengamedev__OpenGLESTransDeathGpuProgramWrapper__

#include "TransDeathGpuProgramWrapper.h"
#include "TransDeathProgram.h"

class OpenGLESTransDeathGpuProgramWrapper : public TransDeathGpuProgramWrapper
{
public:
    OpenGLESTransDeathGpuProgramWrapper(bool isTransIn);
    
    virtual void bind();
    
    virtual void unbind();
    
    virtual void cleanUp();
    
private:
    TransDeathProgramStruct m_program;
};

#endif /* defined(__gowengamedev__OpenGLESTransDeathGpuProgramWrapper__) */
