//
//  SinWaveTextureGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/6/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__SinWaveTextureGpuProgramWrapper__
#define __gowengamedev__SinWaveTextureGpuProgramWrapper__

#include "GpuProgramWrapper.h"

class SinWaveTextureGpuProgramWrapper : public GpuProgramWrapper
{
public:
    SinWaveTextureGpuProgramWrapper();
    
    void setOffset(float offset);
    
protected:
    float m_fOffset;
};

#endif /* defined(__gowengamedev__SinWaveTextureGpuProgramWrapper__) */
