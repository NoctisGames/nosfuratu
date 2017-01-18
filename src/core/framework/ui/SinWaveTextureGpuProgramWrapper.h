//
//  SinWaveTextureGpuProgramWrapper.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 10/6/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__SinWaveTextureGpuProgramWrapper__
#define __noctisgames__SinWaveTextureGpuProgramWrapper__

#include "GpuProgramWrapper.h"

class SinWaveTextureGpuProgramWrapper : public GpuProgramWrapper
{
public:
    SinWaveTextureGpuProgramWrapper();
    
    void setOffset(float offset);
    
protected:
    float m_fOffset;
};

#endif /* defined(__noctisgames__SinWaveTextureGpuProgramWrapper__) */
