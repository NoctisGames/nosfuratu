//
//  OpenGLESSinWaveTextureGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/6/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__OpenGLESSinWaveTextureGpuProgramWrapper__
#define __gowengamedev__OpenGLESSinWaveTextureGpuProgramWrapper__

#include "SinWaveTextureGpuProgramWrapper.h"
#include "SinWaveTextureProgram.h"

class OpenGLESSinWaveTextureGpuProgramWrapper : public SinWaveTextureGpuProgramWrapper
{
public:
    OpenGLESSinWaveTextureGpuProgramWrapper(SinWaveTextureProgramStruct program);
    
    virtual void bind();
    
    virtual void unbind();
    
private:
    SinWaveTextureProgramStruct m_program;
};

#endif /* defined(__gowengamedev__OpenGLESSinWaveTextureGpuProgramWrapper__) */
