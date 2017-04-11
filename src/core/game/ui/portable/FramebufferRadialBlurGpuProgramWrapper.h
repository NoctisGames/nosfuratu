//
//  FramebufferRadialBlurGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/8/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__FramebufferRadialBlurGpuProgramWrapper__
#define __nosfuratu__FramebufferRadialBlurGpuProgramWrapper__

#include "GpuProgramWrapper.h"

class FramebufferRadialBlurGpuProgramWrapper : public GpuProgramWrapper
{
public:
    FramebufferRadialBlurGpuProgramWrapper();
    
    virtual ~FramebufferRadialBlurGpuProgramWrapper();
    
    void configure(float direction);
    
protected:
    float m_fDirection; // Some value between 1.0 and 0.0, where 1.0 is top and 0.0 is bottom
};

#endif /* defined(__nosfuratu__FramebufferRadialBlurGpuProgramWrapper__) */
