//
//  OpenGLESSinWaveTextureGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/6/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__OpenGLESSinWaveTextureGpuProgramWrapper__
#define __noctisgames__OpenGLESSinWaveTextureGpuProgramWrapper__

#include "SinWaveTextureGpuProgramWrapper.h"
#include "SinWaveTextureProgram.h"

class OpenGLESSinWaveTextureGpuProgramWrapper : public SinWaveTextureGpuProgramWrapper
{
public:
    OpenGLESSinWaveTextureGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
    virtual void cleanUp();
    
private:
    SinWaveTextureProgramStruct m_program;
};

#endif /* defined(__noctisgames__OpenGLESSinWaveTextureGpuProgramWrapper__) */
