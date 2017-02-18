//
//  SinWaveTextureGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/6/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__SinWaveTextureGpuProgramWrapper__
#define __nosfuratu__SinWaveTextureGpuProgramWrapper__

#include "GpuProgramWrapper.h"

class SinWaveTextureGpuProgramWrapper : public GpuProgramWrapper
{
public:
    SinWaveTextureGpuProgramWrapper();
    
    virtual ~SinWaveTextureGpuProgramWrapper();
    
    void setOffset(float offset);
    
protected:
    float m_fOffset;
};

#endif /* defined(__nosfuratu__SinWaveTextureGpuProgramWrapper__) */
