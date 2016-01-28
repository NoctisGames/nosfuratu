//
//  OpenGLESTransWorldMapToLevelGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 1/27/16.
//  Copyright (c) 2016 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__OpenGLESTransWorldMapToLevelGpuProgramWrapper__
#define __gowengamedev__OpenGLESTransWorldMapToLevelGpuProgramWrapper__

#include "TransitionGpuProgramWrapper.h"
#include "TransitionProgram.h"

class OpenGLESTransWorldMapToLevelGpuProgramWrapper : public TransitionGpuProgramWrapper
{
public:
    OpenGLESTransWorldMapToLevelGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
    virtual void cleanUp();
    
private:
    TransitionProgramStruct m_program;
};

#endif /* defined(__gowengamedev__OpenGLESTransWorldMapToLevelGpuProgramWrapper__) */
