//
//  OpenGLESTransTitleToWorldGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 1/27/16.
//  Copyright (c) 2016 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__OpenGLESTransTitleToWorldGpuProgramWrapper__
#define __gowengamedev__OpenGLESTransTitleToWorldGpuProgramWrapper__

#include "TransTitleToWorldGpuProgramWrapper.h"
#include "TransTitleToWorldProgram.h"

class OpenGLESTransTitleToWorldGpuProgramWrapper : public TransTitleToWorldGpuProgramWrapper
{
public:
    OpenGLESTransTitleToWorldGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
    virtual void cleanUp();
    
private:
    TransTitleToWorldProgramStruct m_program;
};

#endif /* defined(__gowengamedev__OpenGLESTransTitleToWorldGpuProgramWrapper__) */
